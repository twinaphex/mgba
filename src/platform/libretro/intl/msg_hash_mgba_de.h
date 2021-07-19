#if defined(_MSC_VER) && !defined(_XBOX) && (_MSC_VER >= 1500 && _MSC_VER < 1900)
#if (_MSC_VER >= 1700)
/* https://support.microsoft.com/en-us/kb/980263 */
#pragma execution_character_set("utf-8")
#endif
#pragma warning(disable:4566)
#endif

MSG_HASH(
   MSG_HASH_MGBA_SOLAR_SENSOR_LEVEL_DESC,
   "Solarsensor-Level"
   )
MSG_HASH(
   MSG_HASH_MGBA_SOLAR_SENSOR_LEVEL_INFO,
   ""
   )
MSG_HASH(
   MSG_HASH_OPTION_VAL_USE_DEVICE_SENSOR_IF_AVAILABLE,
   "Gerätesensor verwenden, falls verfügbar"
   )
MSG_HASH(
   MSG_HASH_MGBA_ALLOW_OPPOSING_DIRECTIONS_DESC,
   "Allow Opposing Directional Input"
   )
MSG_HASH(
   MSG_HASH_MGBA_ALLOW_OPPOSING_DIRECTIONS_INFO,
   "Enabling this will allow pressing / quickly alternating / holding both left and right (or up and down) directions at the same time. This may cause movement-based glitches."
   )
MSG_HASH(
   MSG_HASH_MGBA_GB_MODEL_DESC,
   "Game Boy Model (requires restart)"
   )
MSG_HASH(
   MSG_HASH_MGBA_GB_MODEL_INFO,
   "Runs loaded content with a specific Game Boy model. 'Autodetect' will select the most appropriate model for the current game."
   )
MSG_HASH(
   MSG_HASH_OPTION_VAL_AUTODETECT,
   "Autodetect"
   )
MSG_HASH(
   MSG_HASH_OPTION_VAL_GAME_BOY,
   "Game Boy"
   )
MSG_HASH(
   MSG_HASH_OPTION_VAL_SUPER_GAME_BOY,
   "Super Game Boy"
   )
MSG_HASH(
   MSG_HASH_OPTION_VAL_GAME_BOY_COLOR,
   "Game Boy Color"
   )
MSG_HASH(
   MSG_HASH_OPTION_VAL_GAME_BOY_ADVANCE,
   "Game Boy Advance"
   )
MSG_HASH(
   MSG_HASH_MGBA_USE_BIOS_DESC,
   "Use BIOS File if Found (requires restart)"
   )
MSG_HASH(
   MSG_HASH_MGBA_USE_BIOS_INFO,
   "Use official BIOS/bootloader for emulated hardware, if present in RetroArch's system directory."
   )
MSG_HASH(
   MSG_HASH_MGBA_SKIP_BIOS_DESC,
   "Skip BIOS Intro (requires restart)"
   )
MSG_HASH(
   MSG_HASH_MGBA_SKIP_BIOS_INFO,
   "When using an official BIOS/bootloader, skip the start-up logo animation. This setting is ignored when 'Use BIOS File if Found' is disabled."
   )
MSG_HASH(
   MSG_HASH_MGBA_SGB_BORDERS_DESC,
   "Use Super Game Boy Borders (requires restart)"
   )
MSG_HASH(
   MSG_HASH_MGBA_SGB_BORDERS_INFO,
   "Display Super Game Boy borders when running Super Game Boy enhanced games."
   )
MSG_HASH(
   MSG_HASH_MGBA_IDLE_OPTIMIZATION_DESC,
   "Idle Loop Removal"
   )
MSG_HASH(
   MSG_HASH_MGBA_IDLE_OPTIMIZATION_INFO,
   "Reduce system load by optimizing so-called 'idle-loops' - sections in the code where nothing happens, but the CPU runs at full speed (like a car revving in neutral). Improves performance, and should be enabled on low-end hardware."
   )
MSG_HASH(
   MSG_HASH_OPTION_VAL_REMOVE_KNOWN,
   "Remove Known"
   )
MSG_HASH(
   MSG_HASH_OPTION_VAL_DETECT_AND_REMOVE,
   "Detect and Remove"
   )
MSG_HASH(
   MSG_HASH_OPTION_VAL_DONT_REMOVE,
   "Don't Remove"
   )
MSG_HASH(
   MSG_HASH_MGBA_FRAMESKIP_DESC,
   "Frameskip"
   )
MSG_HASH(
   MSG_HASH_MGBA_FRAMESKIP_INFO,
   "Skip frames to avoid audio buffer under-run (crackling). Improves performance at the expense of visual smoothness. 'Auto' skips frames when advised by the frontend. 'Auto (Threshold)' utilises the 'Frameskip Threshold (%)' setting. 'Fixed Interval' utilises the 'Frameskip Interval' setting."
   )
MSG_HASH(
   MSG_HASH_OPTION_VAL_AUTO,
   "Auto"
   )
MSG_HASH(
   MSG_HASH_OPTION_VAL_AUTO_THRESHOLD,
   "Auto (Threshold)"
   )
MSG_HASH(
   MSG_HASH_OPTION_VAL_FIXED_INTERVAL,
   "Fixed Interval"
   )
MSG_HASH(
   MSG_HASH_MGBA_FRAMESKIP_THRESHOLD_DESC,
   "Frameskip Threshold (%)"
   )
MSG_HASH(
   MSG_HASH_MGBA_FRAMESKIP_THRESHOLD_INFO,
   "When 'Frameskip' is set to 'Auto (Threshold)', specifies the audio buffer occupancy threshold (percentage) below which frames will be skipped. Higher values reduce the risk of crackling by causing frames to be dropped more frequently."
   )
MSG_HASH(
   MSG_HASH_MGBA_FRAMESKIP_INTERVAL_DESC,
   "Frameskip Interval"
   )
MSG_HASH(
   MSG_HASH_MGBA_FRAMESKIP_INTERVAL_INFO,
   "When 'Frameskip' is set to 'Fixed Interval', the value set here is the number of frames omitted after a frame is rendered - i.e. '0' = 60fps, '1' = 30fps, '2' = 15fps, etc."
   )
MSG_HASH(
   MSG_HASH_MGBA_COLOR_CORRECTION_DESC,
   "Color Correction"
   )
MSG_HASH(
   MSG_HASH_MGBA_COLOR_CORRECTION_INFO,
   "Adjusts output colors to match the display of real GBA/GBC hardware."
   )
MSG_HASH(
   MSG_HASH_MGBA_INTERFRAME_BLENDING_DESC,
   "Interframe Blending"
   )
MSG_HASH(
   MSG_HASH_MGBA_INTERFRAME_BLENDING_INFO,
   "Simulates LCD ghosting effects. 'Simple' performs a 50:50 mix of the current and previous frames. 'Smart' attempts to detect screen flickering, and only performs a 50:50 mix on affected pixels. 'LCD Ghosting' mimics natural LCD response times by combining multiple buffered frames. 'Simple' or 'Smart' blending is required when playing games that aggressively exploit LCD ghosting for transparency effects (Wave Race, Chikyuu Kaihou Gun ZAS, F-Zero, the Boktai series...)."
   )
MSG_HASH(
   MSG_HASH_OPTION_VAL_SIMPLE,
   "Simple"
   )
MSG_HASH(
   MSG_HASH_OPTION_VAL_SMART,
   "Smart"
   )
MSG_HASH(
   MSG_HASH_OPTION_VAL_LCD_GHOSTING_ACCURATE,
   "LCD Ghosting (Accurate)"
   )
MSG_HASH(
   MSG_HASH_OPTION_VAL_LCD_GHOSTING_FAST,
   "LCD Ghosting (Fast)"
   )
MSG_HASH(
   MSG_HASH_MGBA_FORCE_GBP_DESC,
   "Enable Game Boy Player Rumble (requires restart)"
   )
MSG_HASH(
   MSG_HASH_MGBA_FORCE_GBP_INFO,
   "Enabling this will allow compatible games with the Game Boy Player boot logo to make the controller rumble. Due to how Nintendo decided this feature should work, it may cause glitches such as flickering or lag in some of these games."
   )
MSG_HASH(
   MSG_HASH_MGBA_GB_COLORS_DESC,
   "Set default Game Boy palette"
   )
MSG_HASH(
   MSG_HASH_MGBA_GB_COLORS_INFO,
   "Selects which palette is used for Game Boy games that are not Game Boy Color or Super Game Boy compatible, or if the model is forced to Game Boy."
   )
MSG_HASH(
   MSG_HASH_OPTION_VAL_GRAYSCALE,
   "Grayscale"
   )
MSG_HASH(
   MSG_HASH_OPTION_VAL_DMG_GREEN,
   "DMG Green"
   )
MSG_HASH(
   MSG_HASH_OPTION_VAL_GB_POCKET,
   "GB Pocket"
   )
MSG_HASH(
   MSG_HASH_OPTION_VAL_GB_LIGHT,
   "GB Light"
   )
MSG_HASH(
   MSG_HASH_OPTION_VAL_GBC_BROWN,
   "GBC Brown ↑"
   )
MSG_HASH(
   MSG_HASH_OPTION_VAL_GBC_RED_A,
   "GBC Red ↑A"
   )
MSG_HASH(
   MSG_HASH_OPTION_VAL_GBC_DARK_BROWN_B,
   "GBC Dark Brown ↑B"
   )
MSG_HASH(
   MSG_HASH_OPTION_VAL_GBC_PALE_YELLOW,
   "GBC Pale Yellow ↓"
   )
MSG_HASH(
   MSG_HASH_OPTION_VAL_GBC_ORANGE_A,
   "GBC Orange ↓A"
   )
MSG_HASH(
   MSG_HASH_OPTION_VAL_GBC_YELLOW_B,
   "GBC Yellow ↓B"
   )
MSG_HASH(
   MSG_HASH_OPTION_VAL_GBC_BLUE,
   "GBC Blue ←"
   )
MSG_HASH(
   MSG_HASH_OPTION_VAL_GBC_DARK_BLUE_A,
   "GBC Dark Blue ←A"
   )
MSG_HASH(
   MSG_HASH_OPTION_VAL_GBC_GRAY_B,
   "GBC Gray ←B"
   )
MSG_HASH(
   MSG_HASH_OPTION_VAL_GBC_GREEN,
   "GBC Green →"
   )
MSG_HASH(
   MSG_HASH_OPTION_VAL_GBC_DARK_GREEN_A,
   "GBC Dark Green →A"
   )
MSG_HASH(
   MSG_HASH_OPTION_VAL_GBC_REVERSE_B,
   "GBC Reverse →B"
   )
MSG_HASH(
   MSG_HASH_OPTION_VAL_SGB_1A,
   "SGB 1-A"
   )
MSG_HASH(
   MSG_HASH_OPTION_VAL_SGB_1B,
   "SGB 1-B"
   )
MSG_HASH(
   MSG_HASH_OPTION_VAL_SGB_1C,
   "SGB 1-C"
   )
MSG_HASH(
   MSG_HASH_OPTION_VAL_SGB_1D,
   "SGB 1-D"
   )
MSG_HASH(
   MSG_HASH_OPTION_VAL_SGB_1E,
   "SGB 1-E"
   )
MSG_HASH(
   MSG_HASH_OPTION_VAL_SGB_1F,
   "SGB 1-F"
   )
MSG_HASH(
   MSG_HASH_OPTION_VAL_SGB_1G,
   "SGB 1-G"
   )
MSG_HASH(
   MSG_HASH_OPTION_VAL_SGB_1H,
   "SGB 1-H"
   )
MSG_HASH(
   MSG_HASH_OPTION_VAL_SGB_2A,
   "SGB 2-A"
   )
MSG_HASH(
   MSG_HASH_OPTION_VAL_SGB_2B,
   "SGB 2-B"
   )
MSG_HASH(
   MSG_HASH_OPTION_VAL_SGB_2C,
   "SGB 2-C"
   )
MSG_HASH(
   MSG_HASH_OPTION_VAL_SGB_2D,
   "SGB 2-D"
   )
MSG_HASH(
   MSG_HASH_OPTION_VAL_SGB_2E,
   "SGB 2-E"
   )
MSG_HASH(
   MSG_HASH_OPTION_VAL_SGB_2F,
   "SGB 2-F"
   )
MSG_HASH(
   MSG_HASH_OPTION_VAL_SGB_2G,
   "SGB 2-G"
   )
MSG_HASH(
   MSG_HASH_OPTION_VAL_SGB_2H,
   "SGB 2-H"
   )
MSG_HASH(
   MSG_HASH_OPTION_VAL_SGB_3A,
   "SGB 3-A"
   )
MSG_HASH(
   MSG_HASH_OPTION_VAL_SGB_3B,
   "SGB 3-B"
   )
MSG_HASH(
   MSG_HASH_OPTION_VAL_SGB_3C,
   "SGB 3-C"
   )
MSG_HASH(
   MSG_HASH_OPTION_VAL_SGB_3D,
   "SGB 3-D"
   )
MSG_HASH(
   MSG_HASH_OPTION_VAL_SGB_3E,
   "SGB 3-E"
   )
MSG_HASH(
   MSG_HASH_OPTION_VAL_SGB_3F,
   "SGB 3-F"
   )
MSG_HASH(
   MSG_HASH_OPTION_VAL_SGB_3G,
   "SGB 3-G"
   )
MSG_HASH(
   MSG_HASH_OPTION_VAL_SGB_3H,
   "SGB 3-H"
   )
MSG_HASH(
   MSG_HASH_OPTION_VAL_SGB_4A,
   "SGB 4-A"
   )
MSG_HASH(
   MSG_HASH_OPTION_VAL_SGB_4B,
   "SGB 4-B"
   )
MSG_HASH(
   MSG_HASH_OPTION_VAL_SGB_4C,
   "SGB 4-C"
   )
MSG_HASH(
   MSG_HASH_OPTION_VAL_SGB_4D,
   "SGB 4-D"
   )
MSG_HASH(
   MSG_HASH_OPTION_VAL_SGB_4E,
   "SGB 4-E"
   )
MSG_HASH(
   MSG_HASH_OPTION_VAL_SGB_4F,
   "SGB 4-F"
   )
MSG_HASH(
   MSG_HASH_OPTION_VAL_SGB_4G,
   "SGB 4-G"
   )
MSG_HASH(
   MSG_HASH_OPTION_VAL_SGB_4H,
   "SGB 4-H"
   )
