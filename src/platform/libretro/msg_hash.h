#ifndef __MSG_HASH_H
#define __MSG_HASH_H

#include <stdint.h>
#include <stddef.h>
#include <limits.h>

#define MSG_HASH(Id, str) case Id: return str;

enum msg_hash_enums
{
   MSG_UNKNOWN = 0,

   MSG_HASH_MGBA_SOLAR_SENSOR_LEVEL_DESC,
   MSG_HASH_MGBA_SOLAR_SENSOR_LEVEL_INFO,
   MSG_HASH_OPTION_VAL_USE_DEVICE_SENSOR_IF_AVAILABLE,
   MSG_HASH_MGBA_ALLOW_OPPOSING_DIRECTIONS_DESC,
   MSG_HASH_MGBA_ALLOW_OPPOSING_DIRECTIONS_INFO,
   MSG_HASH_MGBA_GB_MODEL_DESC,
   MSG_HASH_MGBA_GB_MODEL_INFO,
   MSG_HASH_OPTION_VAL_AUTODETECT,
   MSG_HASH_OPTION_VAL_GAME_BOY,
   MSG_HASH_OPTION_VAL_SUPER_GAME_BOY,
   MSG_HASH_OPTION_VAL_GAME_BOY_COLOR,
   MSG_HASH_OPTION_VAL_GAME_BOY_ADVANCE,
   MSG_HASH_MGBA_USE_BIOS_DESC,
   MSG_HASH_MGBA_USE_BIOS_INFO,
   MSG_HASH_MGBA_SKIP_BIOS_DESC,
   MSG_HASH_MGBA_SKIP_BIOS_INFO,
   MSG_HASH_MGBA_SGB_BORDERS_DESC,
   MSG_HASH_MGBA_SGB_BORDERS_INFO,
   MSG_HASH_MGBA_IDLE_OPTIMIZATION_DESC,
   MSG_HASH_MGBA_IDLE_OPTIMIZATION_INFO,
   MSG_HASH_OPTION_VAL_REMOVE_KNOWN,
   MSG_HASH_OPTION_VAL_DETECT_AND_REMOVE,
   MSG_HASH_OPTION_VAL_DONT_REMOVE,
   MSG_HASH_MGBA_FRAMESKIP_DESC,
   MSG_HASH_MGBA_FRAMESKIP_INFO,
   MSG_HASH_OPTION_VAL_AUTO,
   MSG_HASH_OPTION_VAL_AUTO_THRESHOLD,
   MSG_HASH_OPTION_VAL_FIXED_INTERVAL,
   MSG_HASH_MGBA_FRAMESKIP_THRESHOLD_DESC,
   MSG_HASH_MGBA_FRAMESKIP_THRESHOLD_INFO,
   MSG_HASH_MGBA_FRAMESKIP_INTERVAL_DESC,
   MSG_HASH_MGBA_FRAMESKIP_INTERVAL_INFO,
   MSG_HASH_MGBA_COLOR_CORRECTION_DESC,
   MSG_HASH_MGBA_COLOR_CORRECTION_INFO,
   MSG_HASH_MGBA_INTERFRAME_BLENDING_DESC,
   MSG_HASH_MGBA_INTERFRAME_BLENDING_INFO,
   MSG_HASH_OPTION_VAL_SIMPLE,
   MSG_HASH_OPTION_VAL_SMART,
   MSG_HASH_OPTION_VAL_LCD_GHOSTING_ACCURATE,
   MSG_HASH_OPTION_VAL_LCD_GHOSTING_FAST,
   MSG_HASH_MGBA_FORCE_GBP_DESC,
   MSG_HASH_MGBA_FORCE_GBP_INFO,
   MSG_HASH_MGBA_GB_COLORS_DESC,
   MSG_HASH_MGBA_GB_COLORS_INFO,
   MSG_HASH_OPTION_VAL_GRAYSCALE,
   MSG_HASH_OPTION_VAL_DMG_GREEN,
   MSG_HASH_OPTION_VAL_GB_POCKET,
   MSG_HASH_OPTION_VAL_GB_LIGHT,
   MSG_HASH_OPTION_VAL_GBC_BROWN,
   MSG_HASH_OPTION_VAL_GBC_RED_A,
   MSG_HASH_OPTION_VAL_GBC_DARK_BROWN_B,
   MSG_HASH_OPTION_VAL_GBC_PALE_YELLOW,
   MSG_HASH_OPTION_VAL_GBC_ORANGE_A,
   MSG_HASH_OPTION_VAL_GBC_YELLOW_B,
   MSG_HASH_OPTION_VAL_GBC_BLUE,
   MSG_HASH_OPTION_VAL_GBC_DARK_BLUE_A,
   MSG_HASH_OPTION_VAL_GBC_GRAY_B,
   MSG_HASH_OPTION_VAL_GBC_GREEN,
   MSG_HASH_OPTION_VAL_GBC_DARK_GREEN_A,
   MSG_HASH_OPTION_VAL_GBC_REVERSE_B,
   MSG_HASH_OPTION_VAL_SGB_1A,
   MSG_HASH_OPTION_VAL_SGB_1B,
   MSG_HASH_OPTION_VAL_SGB_1C,
   MSG_HASH_OPTION_VAL_SGB_1D,
   MSG_HASH_OPTION_VAL_SGB_1E,
   MSG_HASH_OPTION_VAL_SGB_1F,
   MSG_HASH_OPTION_VAL_SGB_1G,
   MSG_HASH_OPTION_VAL_SGB_1H,
   MSG_HASH_OPTION_VAL_SGB_2A,
   MSG_HASH_OPTION_VAL_SGB_2B,
   MSG_HASH_OPTION_VAL_SGB_2C,
   MSG_HASH_OPTION_VAL_SGB_2D,
   MSG_HASH_OPTION_VAL_SGB_2E,
   MSG_HASH_OPTION_VAL_SGB_2F,
   MSG_HASH_OPTION_VAL_SGB_2G,
   MSG_HASH_OPTION_VAL_SGB_2H,
   MSG_HASH_OPTION_VAL_SGB_3A,
   MSG_HASH_OPTION_VAL_SGB_3B,
   MSG_HASH_OPTION_VAL_SGB_3C,
   MSG_HASH_OPTION_VAL_SGB_3D,
   MSG_HASH_OPTION_VAL_SGB_3E,
   MSG_HASH_OPTION_VAL_SGB_3F,
   MSG_HASH_OPTION_VAL_SGB_3G,
   MSG_HASH_OPTION_VAL_SGB_3H,
   MSG_HASH_OPTION_VAL_SGB_4A,
   MSG_HASH_OPTION_VAL_SGB_4B,
   MSG_HASH_OPTION_VAL_SGB_4C,
   MSG_HASH_OPTION_VAL_SGB_4D,
   MSG_HASH_OPTION_VAL_SGB_4E,
   MSG_HASH_OPTION_VAL_SGB_4F,
   MSG_HASH_OPTION_VAL_SGB_4G,
   MSG_HASH_OPTION_VAL_SGB_4H,
   MSG_LAST,

   /* Ensure sizeof(enum) == sizeof(int) */
   MSG_DUMMY          = INT_MAX
};

static INLINE bool string_is_equal(const char *a, const char *b)
{
   return (a && b) ? !strcmp(a, b) : false;
}

/* Callback strings */

const char *msg_hash_to_str_ar(enum msg_hash_enums msg)
{
   switch (msg)
   {
#include "intl/msg_hash_mgba_ar.h"
      default:
         break;
   }

   return "null";
}

const char *msg_hash_to_str_ast(enum msg_hash_enums msg)
{
   switch (msg)
   {
#include "intl/msg_hash_mgba_ast.h"
      default:
         break;
   }

   return "null";
}

const char *msg_hash_to_str_chs(enum msg_hash_enums msg)
{
   switch (msg)
   {
#include "intl/msg_hash_mgba_chs.h"
      default:
         break;
   }

   return "null";
}
const char *msg_hash_to_str_cht(enum msg_hash_enums msg)
{
   switch (msg)
   {
#include "intl/msg_hash_mgba_cht.h"
      default:
         break;
   }

   return "null";
}

const char *msg_hash_to_str_de(enum msg_hash_enums msg)
{
   switch (msg)
   {
#include "intl/msg_hash_mgba_de.h"
      default:
         break;
   }

   return "null";
}

const char *msg_hash_to_str_el(enum msg_hash_enums msg)
{
   switch (msg)
   {
#include "intl/msg_hash_mgba_el.h"
      default:
         break;
   }

   return "null";
}

const char *msg_hash_to_str_eo(enum msg_hash_enums msg)
{
   switch (msg)
   {
#include "intl/msg_hash_mgba_eo.h"
      default:
         break;
   }

   return "null";
}

const char *msg_hash_to_str_es(enum msg_hash_enums msg)
{
   switch (msg)
   {
#include "intl/msg_hash_mgba_es.h"
      default:
         break;
   }

   return "null";
}

const char *msg_hash_to_str_fa(enum msg_hash_enums msg)
{
   switch (msg)
   {
#include "intl/msg_hash_mgba_fa.h"
      default:
         break;
   }

   return "null";
}

const char *msg_hash_to_str_fi(enum msg_hash_enums msg)
{
   switch (msg)
   {
#include "intl/msg_hash_mgba_fi.h"
      default:
         break;
   }

   return "null";
}

const char *msg_hash_to_str_fr(enum msg_hash_enums msg)
{
   switch (msg)
   {
#include "intl/msg_hash_mgba_fr.h"
      default:
         break;
   }

   return "null";
}

const char *msg_hash_to_str_he(enum msg_hash_enums msg)
{
   switch (msg)
   {
#include "intl/msg_hash_mgba_he.h"
      default:
         break;
   }

   return "null";
}

const char *msg_hash_to_str_it(enum msg_hash_enums msg)
{
   switch (msg)
   {
#include "intl/msg_hash_mgba_it.h"
      default:
         break;
   }

   return "null";
}

const char *msg_hash_to_str_jp(enum msg_hash_enums msg)
{
   switch (msg)
   {
#include "intl/msg_hash_mgba_jp.h"
      default:
         break;
   }

   return "null";
}

const char *msg_hash_to_str_ko(enum msg_hash_enums msg)
{
   switch (msg)
   {
#include "intl/msg_hash_mgba_ko.h"
      default:
         break;
   }

   return "null";
}

const char *msg_hash_to_str_nl(enum msg_hash_enums msg)
{
   switch (msg)
   {
#include "intl/msg_hash_mgba_nl.h"
      default:
         break;
   }

   return "null";
}

const char *msg_hash_to_str_pl(enum msg_hash_enums msg)
{
   switch (msg)
   {
#include "intl/msg_hash_mgba_pl.h"
      default:
         break;
   }

   return "null";
}

const char *msg_hash_to_str_pt_br(enum msg_hash_enums msg)
{
   switch (msg)
   {
#include "intl/msg_hash_mgba_pt_br.h"
      default:
         break;
   }

   return "null";
}

const char *msg_hash_to_str_pt_pt(enum msg_hash_enums msg)
{
   switch (msg)
   {
#include "intl/msg_hash_mgba_pt_pt.h"
      default:
         break;
   }

   return "null";
}

const char *msg_hash_to_str_ru(enum msg_hash_enums msg)
{
   switch (msg)
   {
#include "intl/msg_hash_mgba_ru.h"
      default:
         break;
   }

   return "null";
}

const char *msg_hash_to_str_sk(enum msg_hash_enums msg)
{
   switch (msg)
   {
#include "intl/msg_hash_mgba_sk.h"
      default:
         break;
   }
   return "null";
}

const char *msg_hash_to_str_tr(enum msg_hash_enums msg)
{
   switch (msg)
   {
#include "intl/msg_hash_mgba_tr.h"
      default:
         break;
   }
   return "null";
}

const char *msg_hash_to_str_us(enum msg_hash_enums msg)
{
   switch (msg)
   {
#include "intl/msg_hash_mgba_us.h"
      default:
         break;
   }
   return "null";
}

const char *msg_hash_to_str_vn(enum msg_hash_enums msg)
{
   switch (msg)
   {
#include "intl/msg_hash_mgba_vn.h"
      default:
          break;
   }
   return "null";
}

const char *msg_hash_to_str(enum msg_hash_enums msg, unsigned language)
{
   const char *ret = NULL;

#ifndef HAVE_NO_LANGEXTRA
   switch (language)
   {
      case RETRO_LANGUAGE_ARABIC:
         ret = msg_hash_to_str_ar(msg);
         break;
      case RETRO_LANGUAGE_ASTURIAN:
         ret = msg_hash_to_str_ast(msg);
         break;
      case RETRO_LANGUAGE_CHINESE_SIMPLIFIED:
         ret = msg_hash_to_str_chs(msg);
         break;
      case RETRO_LANGUAGE_CHINESE_TRADITIONAL:
         ret = msg_hash_to_str_cht(msg);
         break;
      case RETRO_LANGUAGE_DUTCH:
         ret = msg_hash_to_str_nl(msg);
         break;
      case RETRO_LANGUAGE_ESPERANTO:
         ret = msg_hash_to_str_eo(msg);
         break;
      case RETRO_LANGUAGE_FINNISH:
         ret = msg_hash_to_str_fi(msg);
         break;
      case RETRO_LANGUAGE_FRENCH:
         ret = msg_hash_to_str_fr(msg);
         break;
      case RETRO_LANGUAGE_GERMAN:
         ret = msg_hash_to_str_de(msg);
         break;
      case RETRO_LANGUAGE_GREEK:
         ret = msg_hash_to_str_el(msg);
         break;
      case RETRO_LANGUAGE_HEBREW:
         ret = msg_hash_to_str_he(msg);
         break;
      case RETRO_LANGUAGE_ITALIAN:
         ret = msg_hash_to_str_it(msg);
         break;
      case RETRO_LANGUAGE_JAPANESE:
         ret = msg_hash_to_str_jp(msg);
         break;
      case RETRO_LANGUAGE_KOREAN:
         ret = msg_hash_to_str_ko(msg);
         break;
      case RETRO_LANGUAGE_PERSIAN:
         ret = msg_hash_to_str_fa(msg);
         break;
      case RETRO_LANGUAGE_POLISH:
         ret = msg_hash_to_str_pl(msg);
         break;
      case RETRO_LANGUAGE_PORTUGUESE_BRAZIL:
         ret = msg_hash_to_str_pt_br(msg);
         break;
      case RETRO_LANGUAGE_PORTUGUESE_PORTUGAL:
         ret = msg_hash_to_str_pt_pt(msg);
         break;
      case RETRO_LANGUAGE_RUSSIAN:
         ret = msg_hash_to_str_ru(msg);
         break;
      case RETRO_LANGUAGE_SLOVAK:
         ret = msg_hash_to_str_sk(msg);
         break;
      case RETRO_LANGUAGE_SPANISH:
         ret = msg_hash_to_str_es(msg);
         break;
      case RETRO_LANGUAGE_TURKISH:
         ret = msg_hash_to_str_tr(msg);
         break;
      case RETRO_LANGUAGE_VIETNAMESE:
         ret = msg_hash_to_str_vn(msg);
         break;
      default:
         break;
   }
#endif

   if (ret && !string_is_equal(ret, "null"))
      return ret;

   return msg_hash_to_str_us(msg);
}

#endif