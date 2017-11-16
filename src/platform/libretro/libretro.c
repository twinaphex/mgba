/* Copyright (c) 2013-2015 Jeffrey Pfau *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "libretro.h"
#include <mgba-util/common.h>
#include <mgba/core/blip_buf.h>
#include <mgba/core/cheats.h>
#include <mgba/core/core.h>
#include <mgba/core/log.h>
#include <mgba/core/version.h>

#ifdef M_CORE_GB
	#include <mgba/gb/core.h>
	#include <mgba/internal/gb/gb.h>
#endif
#ifdef M_CORE_GBA
	#include <mgba/gba/core.h>
	#include <mgba/gba/interface.h>
	#include <mgba/internal/gba/gba.h>
#endif

#include <mgba-util/circle-buffer.h>
#include <mgba-util/memory.h>
#include <mgba-util/vfs.h>

#ifndef __LIBRETRO__
	#error "Can't compile the libretro core as anything other than libretro."
#endif
#ifdef _3DS
	#include <3ds.h>
	FS_Archive sdmcArchive;
#endif

#define SAMPLES		1024
#define RUMBLE_PWM 	35
#define PRESS_BUTTON	inputCallback(0, RETRO_DEVICE_JOYPAD, 0, j)

static retro_environment_t		environCallback;
static retro_video_refresh_t 	   	videoCallback;
static retro_audio_sample_batch_t  	audioCallback;
static retro_input_poll_t 	   	inputPollCallback;
static retro_input_state_t 	   	inputCallback;
static retro_log_printf_t 	   	logCallback;
static retro_set_rumble_state_t    	rumbleCallback;

static void GBARetroLog(struct mLogger* logger, int category, enum mLogLevel level, const char* format, va_list args);
static void _postAudioBuffer(struct mAVStream*, blip_t* left, blip_t* right);
static void _setRumble(struct mRumble* rumble, int enable);
static uint8_t _readLux(struct GBALuminanceSource* lux);
static void _updateLux(struct GBALuminanceSource* lux);

static struct mCore* core;
static struct mAVStream stream;
static void* outputBuffer;
static void* data;
static void* savedata;
static size_t dataSize;
static int rumbleLevel, luxLevel;
static struct CircleBuffer rumbleHistory;
static struct mRumble rumble;
static struct GBALuminanceSource lux;
static struct mLogger logger;

static unsigned int turbo_state = 0, turbo_delay = 5, _direction[4];

static void _reloadSettings(void)
{
	struct mCoreOptions opts = { .useBios = true, .volume = 0x100, };
	struct retro_variable var;

	var.key = "mgba_use_bios";
	var.value = 0;
	if (environCallback(RETRO_ENVIRONMENT_GET_VARIABLE, &var) && var.value)
		opts.useBios = strcmp(var.value, "ON") == 0;

	var.key = "mgba_skip_bios";
	var.value = 0;
	if (environCallback(RETRO_ENVIRONMENT_GET_VARIABLE, &var) && var.value)
		opts.skipBios = strcmp(var.value, "ON") == 0;

	var.key = "mgba_solar_sensor_level";
	var.value = 0;
	if (environCallback(RETRO_ENVIRONMENT_GET_VARIABLE, &var) && var.value)
		luxLevel = atoi(var.value);

	var.key = "mgba_idle_optimization";
	var.value = 0;
	if (environCallback(RETRO_ENVIRONMENT_GET_VARIABLE, &var) && var.value)
	{
		if (strcmp(var.value, "Don't Remove") == 0)
			mCoreConfigSetDefaultValue(&core->config, "idleOptimization", "ignore");
		else if (strcmp(var.value, "Remove Known") == 0)
			mCoreConfigSetDefaultValue(&core->config, "idleOptimization", "remove");
		else if (strcmp(var.value, "Detect and Remove") == 0)
			mCoreConfigSetDefaultValue(&core->config, "idleOptimization", "detect");
	}

	var.key = "mgba_turbo_delay";
	var.value = 0;
	if (environCallback(RETRO_ENVIRONMENT_GET_VARIABLE, &var) && var.value)
	{
		if (strcmp(var.value, "Medium") == 0)
			turbo_delay = 6;
		else if (strcmp(var.value, "Slow") == 0)
			turbo_delay = 9;
		else
			turbo_delay = 3;
	}

	var.key = "mgba_allow_opposing_directions";
	var.value = 0;
	if (environCallback(RETRO_ENVIRONMENT_GET_VARIABLE, &var) && var.value)
	{
		unsigned int i, j, _temp[4][4] = { { 7, 6, 5, 4 }, { 6 ,7, 4, 5 }, { 7, 6, 4, 5 }, { 6, 7, 5, 4 } };
		if (strcmp(var.value, "Up & Down") == 0)
			j = 0;
		else if (strcmp(var.value, "Left & Right") == 0)
			j = 1;
		else if (strcmp(var.value, "All directions") == 0)
			j = 2;
		else
			j = 3;

		for (i = 0; i < 4; i++)
			_direction[i] = _temp[j][i];
	}

	var.key = "mgba_set_volume";
	var.value = 0;
	if (environCallback(RETRO_ENVIRONMENT_GET_VARIABLE, &var) && var.value)
	{
		unsigned int temp = atoi(var.value);
		opts.volume = (unsigned int)( 2.56f * temp);
	}

	mCoreConfigLoadDefaults(&core->config, &opts);
	mCoreLoadConfig(core);
}

unsigned retro_api_version(void) { return RETRO_API_VERSION; }

void retro_set_environment(retro_environment_t env)
{
	environCallback = env;
	struct retro_variable vars[] = {
		{ "mgba_solar_sensor_level",		"Solar sensor level; 0|1|2|3|4|5|6|7|8|9|10" },
		{ "mgba_turbo_delay", 			"Turbo delay; Medium|Slow|Fast" },
		{ "mgba_set_volume", 			"Sound volume; 80%|85%|90%|95%|100%|10%|20%|30%|40%|50%|60%|70%|75%" },
		{ "mgba_allow_opposing_directions",	"Allow opposing directional input; disabled|Up & Down|Left & Right|All directions" },
		{ "mgba_use_bios", 		     	"Use BIOS file if found; ON|OFF" },
		{ "mgba_skip_bios", 		     	"Skip BIOS intro; OFF|ON" },
		{ "mgba_idle_optimization",		"Idle loop removal; Remove Known|Detect and Remove|Don't Remove" },
		{ 0, 0 }
	};
	environCallback(RETRO_ENVIRONMENT_SET_VARIABLES, vars);
}

void retro_set_video_refresh(retro_video_refresh_t video) { videoCallback = video; }
void retro_set_audio_sample(retro_audio_sample_t audio) { UNUSED(audio); }
void retro_set_audio_sample_batch(retro_audio_sample_batch_t audioBatch) { audioCallback = audioBatch; }
void retro_set_input_poll(retro_input_poll_t inputPoll) { inputPollCallback = inputPoll; }
void retro_set_input_state(retro_input_state_t input) { inputCallback = input; }

void retro_get_system_info(struct retro_system_info* info)
{
	info->need_fullpath = false;
	info->valid_extensions = "gba|gb|gbc";
#ifndef GIT_VERSION
	#define GIT_VERSION ""
#endif
	info->library_version = "0.6.1" GIT_VERSION;
	info->library_name = "mGBA";
	info->block_extract = false;
}

void retro_get_system_av_info(struct retro_system_av_info* info)
{
	unsigned width, height;

	core->desiredVideoDimensions(core, &width, &height);

	info->geometry.base_width = width;
	info->geometry.base_height = height;
	info->geometry.max_width = width;
	info->geometry.max_height = height;
	info->geometry.aspect_ratio = width / (double) height;
	info->timing.fps = core->frequency(core) / (float) core->frameCycles(core);
	info->timing.sample_rate = 32768;
}

void retro_init(void)
{
	enum retro_pixel_format fmt;

#ifdef COLOR_16_BIT
   #ifdef COLOR_5_6_5
	fmt = RETRO_PIXEL_FORMAT_RGB565;
   #else
	#warning This pixel format is unsupported. Please use -DCOLOR_16-BIT -DCOLOR_5_6_5
	fmt = RETRO_PIXEL_FORMAT_0RGB1555;
   #endif
#else
	#warning This pixel format is unsupported. Please use -DCOLOR_16-BIT -DCOLOR_5_6_5
	fmt = RETRO_PIXEL_FORMAT_XRGB8888;
#endif

	environCallback(RETRO_ENVIRONMENT_SET_PIXEL_FORMAT, &fmt);

	struct retro_input_descriptor inputDescriptors[] = {
		{ 0, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_A, "A" },
		{ 0, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_B, "B" },
		{ 0, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_X, "Turbo A" },
		{ 0, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_Y, "Turbo B" },
		{ 0, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_SELECT, "Select" },
		{ 0, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_START, "Start" },
		{ 0, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_RIGHT, "Right" },
		{ 0, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_LEFT, "Left" },
		{ 0, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_UP, "Up" },
		{ 0, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_DOWN, "Down" },
		{ 0, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_R, "R" },
		{ 0, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_L, "L" },
		{ 0, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_R2, "Turbo R" },
		{ 0, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_L2, "Turbo L" },
		{ 0, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_R3, "Brighten Solar Sensor" },
		{ 0, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_L3, "Darken Solar Sensor" },
		{ 0, 0, 0, 0, NULL }
	};
	environCallback(RETRO_ENVIRONMENT_SET_INPUT_DESCRIPTORS, &inputDescriptors);

	// TODO: RETRO_ENVIRONMENT_SET_SUPPORT_NO_GAME when BIOS booting is supported

	struct retro_rumble_interface rumbleInterface;
	if (environCallback(RETRO_ENVIRONMENT_GET_RUMBLE_INTERFACE, &rumbleInterface))
	{
		rumbleCallback = rumbleInterface.set_rumble_state;
		CircleBufferInit(&rumbleHistory, RUMBLE_PWM);
		rumble.setRumble = _setRumble;
	}
	else
		rumbleCallback = 0;

	luxLevel = 0;
	lux.readLuminance = _readLux;
	lux.sample = _updateLux;
	_updateLux(&lux);

	struct retro_log_callback log;
	if (environCallback(RETRO_ENVIRONMENT_GET_LOG_INTERFACE, &log))
		logCallback = log.log;
	else
		logCallback = 0;

	logger.log = GBARetroLog;
	mLogSetDefaultLogger(&logger);

	stream.videoDimensionsChanged = 0;
	stream.postAudioFrame = 0;
	stream.postAudioBuffer = _postAudioBuffer;
	stream.postVideoFrame = 0;
}

void retro_deinit(void)
{
#ifdef _3DS
	linearFree(outputBuffer);
#else
	free(outputBuffer);
#endif
}

static void input_update(void)
{
	inputPollCallback();

	uint16_t keys = 0;
	register unsigned int j;

	for (j = RETRO_DEVICE_ID_JOYPAD_B; j < RETRO_DEVICE_ID_JOYPAD_L3; j++)
	{
		if (PRESS_BUTTON)
		{
			switch (j) {
			case RETRO_DEVICE_ID_JOYPAD_RIGHT:
				keys |= 1 << _direction[3]; break;
			case RETRO_DEVICE_ID_JOYPAD_LEFT:
				keys |= 1 << _direction[2]; break;
			case RETRO_DEVICE_ID_JOYPAD_UP:
				keys |= 1 << _direction[0]; break;
			case RETRO_DEVICE_ID_JOYPAD_DOWN:
				keys |= 1 << _direction[1]; break;
			case RETRO_DEVICE_ID_JOYPAD_A:
				keys |= 1 << 0; break;
			case RETRO_DEVICE_ID_JOYPAD_B:
				keys |= 1 << 1; break;
			case RETRO_DEVICE_ID_JOYPAD_SELECT:
				keys |= 1 << 2; break;
			case RETRO_DEVICE_ID_JOYPAD_START:
				keys |= 1 << 3; break;
			case RETRO_DEVICE_ID_JOYPAD_R:
				keys |= 1 << 8; break;
			case RETRO_DEVICE_ID_JOYPAD_L:
				keys |= 1 << 9; break;
			case RETRO_DEVICE_ID_JOYPAD_Y:
				if (turbo_state >= turbo_delay)
					keys |= 1 << 1;
			break;
			case RETRO_DEVICE_ID_JOYPAD_X:
				if (turbo_state >= turbo_delay)
					keys |= 1 << 0;
			break;
			case RETRO_DEVICE_ID_JOYPAD_L2:
				if (turbo_state >= turbo_delay)
					keys |= 1 << 9;
			break;
			case RETRO_DEVICE_ID_JOYPAD_R2:
				if (turbo_state >= turbo_delay)
					keys |= 1 << 8;
			break; }
		}
		core->setKeys(core, keys);
	}
}

void retro_run(void)
{
	bool updated = false;
	if (environCallback(RETRO_ENVIRONMENT_GET_VARIABLE_UPDATE, &updated) && updated)
		_reloadSettings();

	input_update();

	static bool wasAdjustingLux = false;
	if (wasAdjustingLux)
		wasAdjustingLux = inputCallback(0, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_R3) || inputCallback(0, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_L3);
	else
	{
		if (inputCallback(0, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_R3))
		{
			luxLevel > 9 ? luxLevel = 10 : luxLevel++;
			wasAdjustingLux = true;
		}
		else if (inputCallback(0, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_L3))
		{
			luxLevel < 1 ? luxLevel = 0 : luxLevel--;
			wasAdjustingLux = true;
		}
	}

	core->runFrame(core);
	unsigned int width, height;
	core->desiredVideoDimensions(core, &width, &height);
	videoCallback(outputBuffer, width, height, BYTES_PER_PIXEL << 8);

	turbo_state > turbo_delay ? turbo_state = 0 : turbo_state++;
}

void static _setupMaps(struct mCore* core)
{
#ifdef M_CORE_GBA
	if (core->platform(core) == PLATFORM_GBA)
	{
		struct GBA* gba = core->board;
		struct retro_memory_descriptor descs[11];
		struct retro_memory_map mmaps;
		size_t romSize = gba->memory.romSize + (gba->memory.romSize & 1);

		memset(descs, 0, sizeof(descs));
		size_t savedataSize = retro_get_memory_size(RETRO_MEMORY_SAVE_RAM);

		/* Map internal working RAM */
		descs[0].ptr    = gba->memory.iwram;
		descs[0].start  = BASE_WORKING_IRAM;
		descs[0].len    = SIZE_WORKING_IRAM;
		descs[0].select = 0xFF000000;

		/* Map working RAM */
		descs[1].ptr    = gba->memory.wram;
		descs[1].start  = BASE_WORKING_RAM;
		descs[1].len    = SIZE_WORKING_RAM;
		descs[1].select = 0xFF000000;

		/* Map save RAM */
		/* TODO: if SRAM is flash, use start=0 addrspace="S" instead */
		descs[2].ptr    = savedataSize ? savedata : NULL;
		descs[2].start  = BASE_CART_SRAM;
		descs[2].len    = savedataSize;

		/* Map ROM */
		descs[3].ptr    = gba->memory.rom;
		descs[3].start  = BASE_CART0;
		descs[3].len    = romSize;
		descs[3].flags  = RETRO_MEMDESC_CONST;

		descs[4].ptr    = gba->memory.rom;
		descs[4].start  = BASE_CART1;
		descs[4].len    = romSize;
		descs[4].flags  = RETRO_MEMDESC_CONST;

		descs[5].ptr    = gba->memory.rom;
		descs[5].start  = BASE_CART2;
		descs[5].len    = romSize;
		descs[5].flags  = RETRO_MEMDESC_CONST;

		/* Map BIOS */
		descs[6].ptr    = gba->memory.bios;
		descs[6].start  = BASE_BIOS;
		descs[6].len    = SIZE_BIOS;
		descs[6].flags  = RETRO_MEMDESC_CONST;

		/* Map VRAM */
		descs[7].ptr    = gba->video.vram;
		descs[7].start  = BASE_VRAM;
		descs[7].len    = SIZE_VRAM;
		descs[7].select = 0xFF000000;

		/* Map palette RAM */
		descs[8].ptr    = gba->video.palette;
		descs[8].start  = BASE_PALETTE_RAM;
		descs[8].len    = SIZE_PALETTE_RAM;
		descs[8].select = 0xFF000000;

		/* Map OAM */
		descs[9].ptr    = &gba->video.oam; /* video.oam is a structure */
		descs[9].start  = BASE_OAM;
		descs[9].len    = SIZE_OAM;
		descs[9].select = 0xFF000000;

		/* Map mmapped I/O */
		descs[10].ptr    = gba->memory.io;
		descs[10].start  = BASE_IO;
		descs[10].len    = SIZE_IO;

		mmaps.descriptors = descs;
		mmaps.num_descriptors = sizeof(descs) / sizeof(descs[0]);

		bool yes = true;
		environCallback(RETRO_ENVIRONMENT_SET_MEMORY_MAPS, &mmaps);
		environCallback(RETRO_ENVIRONMENT_SET_SUPPORT_ACHIEVEMENTS, &yes);
	}
#endif
}


void retro_reset(void)
{
	core->reset(core);
	_setupMaps(core);

	if (rumbleCallback)
		CircleBufferClear(&rumbleHistory);
}


bool retro_load_game(const struct retro_game_info* game)
{
	struct VFile* rom;

	if (!game) return false;

	if (game->data)
	{
		data = anonymousMemoryMap(game->size);
		dataSize = game->size;
		memcpy(data, game->data, game->size);
		rom = VFileFromMemory(data, game->size);
	}
	else
	{
		data = 0;
		rom = VFileOpen(game->path, O_RDONLY);
	}

	if (!rom) return false;

	core = mCoreFindVF(rom);

	if (!core)
	{
		rom->close(rom);
		mappedMemoryFree(data, game->size);
		return false;
	}
	mCoreInitConfig(core, NULL);
	core->init(core);
	core->setAVStream(core, &stream);

#ifdef _3DS
	outputBuffer = linearMemAlign(256 * VIDEO_VERTICAL_PIXELS * BYTES_PER_PIXEL, 0x80);
#else
	outputBuffer = malloc(VIDEO_VERTICAL_PIXELS * BYTES_PER_PIXEL * 256);
#endif
	core->setVideoBuffer(core, outputBuffer, 256);

	core->setAudioBufferSize(core, SAMPLES);

	blip_set_rates(core->getAudioChannel(core, 0), core->frequency(core), 32768);
	blip_set_rates(core->getAudioChannel(core, 1), core->frequency(core), 32768);

	core->setPeripheral(core, mPERIPH_RUMBLE, &rumble);

	savedata = anonymousMemoryMap(SIZE_CART_FLASH1M);
	struct VFile* save = VFileFromMemory(savedata, SIZE_CART_FLASH1M);

	_reloadSettings();

	core->loadROM(core, rom);
	core->loadSave(core, save);

#ifdef M_CORE_GBA
	if (core->platform(core) == PLATFORM_GBA)
	{
		core->setPeripheral(core, mPERIPH_GBA_LUMINANCE, &lux);

		const char* sysDir = 0;
		if (core->opts.useBios && environCallback(RETRO_ENVIRONMENT_GET_SYSTEM_DIRECTORY, &sysDir))
		{
			char biosPath[PATH_MAX];
			snprintf(biosPath, sizeof(biosPath), "%s%s%s", sysDir, PATH_SEP, "gba_bios.bin");
			struct VFile* bios = VFileOpen(biosPath, O_RDONLY);
			if (bios)
				core->loadBIOS(core, bios, 0);
		}
	}
#endif
	core->reset(core);
	_setupMaps(core);

	return true;
}

void retro_unload_game(void) {
	if (!core)
		return;
	core->deinit(core);

	mappedMemoryFree(data, dataSize);
	data = 0;

	mappedMemoryFree(savedata, SIZE_CART_FLASH1M);
	savedata = 0;

	CircleBufferDeinit(&rumbleHistory);
}

size_t retro_serialize_size(void) { return core->stateSize(core); }

bool retro_serialize(void* data, size_t size)
{
	if (size != retro_serialize_size())
		return false;

	core->saveState(core, data);
	return true;
}

bool retro_unserialize(const void* data, size_t size)
{
	if (size != retro_serialize_size())
		return false;

	core->loadState(core, data);
	return true;
}

void retro_cheat_reset(void) { mCheatDeviceClear(core->cheatDevice(core)); }

void retro_cheat_set(unsigned index, bool enabled, const char* code)
{
	UNUSED(index);
	UNUSED(enabled);

	struct mCheatDevice* device = core->cheatDevice(core);
	struct mCheatSet* cheatSet = NULL;

	if (mCheatSetsSize(&device->cheats))
		cheatSet = *mCheatSetsGetPointer(&device->cheats, 0);
	else
	{
		cheatSet = device->createSet(device, NULL);
		mCheatAddSet(device, cheatSet);
	}

	// Convert the super wonky unportable libretro format to something normal
	char realCode[] = "XXXXXXXX XXXXXXXX";
	size_t i, pos, len = strlen(code) + 1; 		// Include null terminator
	for (i = 0, pos = 0; i < len; ++i)
	{
		if (isspace((int) code[i]) || code[i] == '+')
			realCode[pos] = ' ';
		else
			realCode[pos] = code[i];

		if ((pos == 13 && (realCode[pos] == ' ' || !realCode[pos])) || pos == 17)
		{
			realCode[pos] = '\0';
			mCheatAddLine(cheatSet, realCode, 0);
			pos = 0;
			continue;
		}
		++pos;
	}
}

// TODO: This isn't strictly true
unsigned retro_get_region(void) { return RETRO_REGION_NTSC; }

void retro_set_controller_port_device(unsigned port, unsigned device)
{
	UNUSED(port);
	UNUSED(device);
}

bool retro_load_game_special(unsigned game_type, const struct retro_game_info* info, size_t num_info)
{
	UNUSED(game_type);
	UNUSED(info);
	UNUSED(num_info);
	return false;
}

void* retro_get_memory_data(unsigned id)
{
	struct GBA* gba = core->board;
	struct GB* gb = core->board;

	switch (id)
	{
		case RETRO_MEMORY_SAVE_RAM:
			return savedata;

		case RETRO_MEMORY_SYSTEM_RAM:
		{
			if (core->platform(core) == PLATFORM_GBA)
				return gba->memory.wram;

			if (core->platform(core) == PLATFORM_GB)
				return gb->memory.wram;
		}

		case RETRO_MEMORY_VIDEO_RAM:
		{
			if (core->platform(core) == PLATFORM_GBA)
				return gba->video.renderer->vram;

			if (core->platform(core) == PLATFORM_GB)
				return gb->video.renderer->vram;
		}
	}
	return 0;
}

size_t retro_get_memory_size(unsigned id)
{
	switch (id)
	{
		case RETRO_MEMORY_SAVE_RAM:
		{
#ifdef M_CORE_GBA
			if (core->platform(core) == PLATFORM_GBA)
			{
				switch (((struct GBA*) core->board)->memory.savedata.type)
				{
					case SAVEDATA_AUTODETECT:
						return SIZE_CART_FLASH1M;
					default:
						return GBASavedataSize(&((struct GBA*) core->board)->memory.savedata);
				}
			}
#endif
#ifdef M_CORE_GB
			if (core->platform(core) == PLATFORM_GB)
				return ((struct GB*) core->board)->sramSize;
#endif
		}
		case RETRO_MEMORY_SYSTEM_RAM:
			return SIZE_WORKING_RAM;
		case RETRO_MEMORY_VIDEO_RAM:
			return SIZE_VRAM;
	}
	return 0;
}

void GBARetroLog(struct mLogger* logger, int category, enum mLogLevel level, const char* format, va_list args)
{
	UNUSED(logger);

	if (!logCallback) return;

	char message[128];
	vsnprintf(message, sizeof(message), format, args);
	enum retro_log_level retroLevel = RETRO_LOG_INFO;

	switch (level)
	{
		case mLOG_ERROR:
		case mLOG_FATAL:
			retroLevel = RETRO_LOG_ERROR;
		break;
		case mLOG_WARN:
			retroLevel = RETRO_LOG_WARN;
		break;
		case mLOG_INFO:
			retroLevel = RETRO_LOG_INFO;
		break;
		case mLOG_GAME_ERROR:
		case mLOG_STUB:
#ifdef NDEBUG
		return;
#else
			retroLevel = RETRO_LOG_DEBUG;
		break;
#endif
		case mLOG_DEBUG:
			retroLevel = RETRO_LOG_DEBUG;
		break;
	}
#ifdef NDEBUG
	static int biosCat = -1;
	if (biosCat < 0)
		biosCat = mLogCategoryById("gba.bios");

	if (category == biosCat)
		return;
#endif
	logCallback(retroLevel, "%s: %s\n", mLogCategoryName(category), message);
}

static void _postAudioBuffer(struct mAVStream* stream, blip_t* left, blip_t* right)
{
	UNUSED(stream);

	int16_t samples[SAMPLES * 2];
	blip_read_samples(left, samples, SAMPLES, true);
	blip_read_samples(right, samples + 1, SAMPLES, true);
	audioCallback(samples, SAMPLES);
}

static void _setRumble(struct mRumble* rumble, int enable)
{
	UNUSED(rumble);

	if (!rumbleCallback) return;

	rumbleLevel += enable;
	if (CircleBufferSize(&rumbleHistory) == RUMBLE_PWM)
	{
		int8_t oldLevel;
		CircleBufferRead8(&rumbleHistory, &oldLevel);
		rumbleLevel -= oldLevel;
	}
	CircleBufferWrite8(&rumbleHistory, enable);

	rumbleCallback(0, RETRO_RUMBLE_STRONG, rumbleLevel * 0xFFFF / RUMBLE_PWM);
	rumbleCallback(0, RETRO_RUMBLE_WEAK, rumbleLevel * 0xFFFF / RUMBLE_PWM);
}

static void _updateLux(struct GBALuminanceSource* lux)
{
	UNUSED(lux);

	struct retro_variable var = { .key = "mgba_solar_sensor_level", .value = 0 };

	bool updated = false;
	if (!environCallback(RETRO_ENVIRONMENT_GET_VARIABLE, &var) || !var.value) return;
	if (!environCallback(RETRO_ENVIRONMENT_GET_VARIABLE_UPDATE, &updated) || !updated) return;

	char* end;
	int newLuxLevel = strtol(var.value, &end, 10);
	if (!*end)
	{
		if (newLuxLevel > 10)
			newLuxLevel = 10;
		if (newLuxLevel < 0)
			newLuxLevel = 0;

		luxLevel = newLuxLevel;
	}
}

static uint8_t _readLux(struct GBALuminanceSource* lux)
{
	UNUSED(lux);

	int value = 0x16;
	if (luxLevel > 0)
		value += GBA_LUX_LEVELS[luxLevel - 1];
	return 0xFF - value;
}
