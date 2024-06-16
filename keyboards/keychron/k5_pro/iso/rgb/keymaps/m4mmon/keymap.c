/* Copyright 2023 @ Keychron (https://www.keychron.com)
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include QMK_KEYBOARD_H

#include "lib/lib8tion/lib8tion.h"

#define DPRINT if (debug_enable) uprintf
// clang-format off
#ifdef INDICATOR_COLOR_ENABLE

#if !defined(RGB_MATRIX_HUE_STEP)
#    define RGB_MATRIX_HUE_STEP 8
#endif

#if !defined(RGB_MATRIX_SAT_STEP)
#    define RGB_MATRIX_SAT_STEP 16
#endif

#if !defined(RGB_MATRIX_VAL_STEP)
#    define RGB_MATRIX_VAL_STEP 16
#endif

typedef union {
  uint32_t raw;
  struct {
    HSV hsv;
  };
} user_config_t;

user_config_t user_config;
RGB gRgb = { 255, 255, 255 };

#endif

enum custom_keycodes
{
   KC_INDICATOR_DH = QK_KB_2,
   KC_INDICATOR_UH,
   KC_INDICATOR_DS,
   KC_INDICATOR_US,
   KC_INDICATOR_DV,
   KC_INDICATOR_UV
};

#define KC_INDH KC_INDICATOR_DH
#define KC_INUH KC_INDICATOR_UH
#define KC_INDS KC_INDICATOR_DS
#define KC_INUS KC_INDICATOR_US
#define KC_INDV KC_INDICATOR_DV
#define KC_INUV KC_INDICATOR_UV

#define MYCOMP LGUI(KC_E)
#define OS_SET LGUI(KC_I)
#define OSTASK RCS(KC_ESC)


#ifdef DIP_SWITCH_ENABLE
static bool bDipSwitchState = FALSE;
#endif

enum layers
{
    WIN_BASE,
    WIN_FN,
};

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] =
{
    [WIN_BASE] = LAYOUT_109_iso(
        KC_ESC,             KC_F1,    KC_F2,    KC_F3,    KC_F4,    KC_F5,    KC_F6,    KC_F7,    KC_F8,    KC_F9,    KC_F10,   KC_F11,   KC_F12,  KC_PSCR,  KC_SCRL,  KC_PAUS,  KC_WHOM,  MYCOMP,   OS_SET,   OSTASK,
        KC_GRV,   KC_1,     KC_2,     KC_3,     KC_4,     KC_5,     KC_6,     KC_7,     KC_8,     KC_9,     KC_0,     KC_MINS,  KC_EQL,   KC_BSPC, KC_INS,   KC_HOME,  KC_PGUP,  KC_NUM,   KC_PSLS,  KC_PAST,  KC_PMNS,
        KC_TAB,   KC_Q,     KC_W,     KC_E,     KC_R,     KC_T,     KC_Y,     KC_U,     KC_I,     KC_O,     KC_P,     KC_LBRC,  KC_RBRC,           KC_DEL,   KC_END,   KC_PGDN,  KC_P7,    KC_P8,    KC_P9,    KC_PPLS,
        KC_CAPS,  KC_A,     KC_S,     KC_D,     KC_F,     KC_G,     KC_H,     KC_J,     KC_K,     KC_L,     KC_SCLN,  KC_QUOT,  KC_NUHS,  KC_ENT,                                KC_P4,    KC_P5,    KC_P6,
        KC_LSFT,  KC_NUBS,  KC_Z,     KC_X,     KC_C,     KC_V,     KC_B,     KC_N,     KC_M,     KC_COMM,  KC_DOT,   KC_SLSH,            KC_RSFT,           KC_UP,              KC_P1,    KC_P2,    KC_P3,    KC_PENT,
        KC_LCTL,  KC_LWIN,  KC_LALT,                                KC_SPC,                                 KC_RALT,  KC_APP,  MO(WIN_FN),KC_RCTL, KC_LEFT,  KC_DOWN,  KC_RGHT,  KC_P0,              KC_PDOT        ),
    [WIN_FN] = LAYOUT_109_iso(
        _______,            KC_BRID,  KC_BRIU,  KC_TASK,  KC_FILE,  RGB_VAD,  RGB_VAI,  KC_MPRV,  KC_MPLY,  KC_MNXT,  KC_MUTE,  KC_VOLD,  KC_VOLU, _______,  _______,  _______,  EE_CLR,   DB_TOGG,  QK_BOOT,  QK_RBT,
        _______,  BT_HST1,  BT_HST2,  BT_HST3,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______, KC_INUH,  KC_INUS,  KC_INUV,  _______,  _______,  _______,  _______,
        RGB_TOG,  RGB_MOD,  RGB_VAI,  RGB_HUI,  RGB_SAI,  RGB_SPI,  _______,  _______,  _______,  _______,  _______,  _______,  _______,           KC_INDH,  KC_INDS,  KC_INDV,  _______,  _______,  _______,  _______,
        _______,  RGB_RMOD, RGB_VAD,  RGB_HUD,  RGB_SAD,  RGB_SPD,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,                               _______,  _______,  _______,
        _______,  _______,  _______,  _______,  _______,  _______,  BAT_LVL,  NK_TOGG,  _______,  _______,  _______,  _______,            _______,           _______,            _______,  _______,  _______,  _______,
        _______,  _______,  _______,                                _______,                                _______,  KC_RWIN,  _______,  _______, _______,  _______,  _______,  _______,            _______        ),
};

// clang-format on

#ifdef INDICATOR_COLOR_ENABLE
// -----------------------------------------------------------------------------

// for use within indicator led update function

void getIndicatorRGB(RGB * pRgb)
{
   pRgb->r = gRgb.r;
   pRgb->g = gRgb.g;
   pRgb->b = gRgb.b;
}

// -----------------------------------------------------------------------------

void UpdateHSV(uint8_t h, uint8_t s, uint8_t v)
{
   // only do something if a value will change

   if (user_config.hsv.h != h || user_config.hsv.s != s || user_config.hsv.v != v)
   {
      user_config.hsv.h = h;
      user_config.hsv.s = s;
      user_config.hsv.v = v;

      // compute rgb values for indicator usage

      gRgb = hsv_to_rgb(user_config.hsv);

      // persist in eeprom

      eeconfig_update_user(user_config.raw);
#ifdef CONSOLE_ENABLE

      DPRINT(
         "[persisted] indicator h=0x%02X s=0x%02X v=0x%02X / r=0x%02X g=0x%02X b=0x%02X\n",
         user_config.hsv.h, user_config.hsv.s, user_config.hsv.v,
         gRgb.r, gRgb.g, gRgb.b
      );
   }
   else 
   {
      DPRINT("indicator hsv unchanged\n");
#endif
   }
}

// -----------------------------------------------------------------------------

void eeconfig_init_user(void)
{
   // initialize structure. called at first boot after reset

   user_config.raw = 0;

   // default indicator color

   UpdateHSV(0xAF, 0xFF, 0x9F);
}
#endif

// -----------------------------------------------------------------------------

void keyboard_post_init_user(void)
{
   default_layer_set(0);
#ifdef CONSOLE_ENABLE
   // automatically enable output

   debug_enable = true;
#endif
#ifdef INDICATOR_COLOR_ENABLE
   // read structure from eeprom at boot time
   
   user_config.raw = eeconfig_read_user();

   // compute corresponding rgb

   gRgb = hsv_to_rgb(user_config.hsv);
#ifdef CONSOLE_ENABLE

   DPRINT(
      "[init from eeprom] indicator h=0x%02X s=0x%02X v=0x%02X / r=0x%02X g=0x%02X b=0x%02X\n",
      user_config.hsv.h, user_config.hsv.s, user_config.hsv.v,
      gRgb.r, gRgb.g, gRgb.b
   );
#endif // CONSOLE_ENABLE
#endif
}

// -----------------------------------------------------------------------------

bool process_record_user(uint16_t keycode, keyrecord_t *record)
{
   bool bAllowFurtherProcessing = true;
#ifdef CONSOLE_ENABLE

   DPRINT(
      "process_record_user: kc: 0x%04X, col: %2u, row: %2u, pressed: %u, time: %5u, int: %u, count: %u\n",
      keycode,
      record->event.key.col,
      record->event.key.row,
      record->event.pressed,
      record->event.time,
      record->tap.interrupted,
      record->tap.count
   );
#endif

   if (!record->event.pressed)
   {
      if (keycode == KC_NUM_LOCK)
      {
          // if dip switch active, drop numlock keypress
          if (bDipSwitchState)
          {
              unregister_code(KC_F13);
              bAllowFurtherProcessing = false;
          }
      }
   }
   else
   {
      if (keycode == KC_NUM_LOCK)
      {
          // if dip switch active, drop numlock keypress
          if (bDipSwitchState)
          {
              register_code(KC_F13);
              bAllowFurtherProcessing = false;
          }
      }
#ifdef INDICATOR_COLOR_ENABLE
      // ----------------------- HUE

      // decrease hue

      else if (keycode == KC_INDH)
      {
         UpdateHSV(
            user_config.hsv.h - RGB_MATRIX_HUE_STEP, // overflow is wanted
            user_config.hsv.s,
            user_config.hsv.v
         );
         bAllowFurtherProcessing = false;
      }
      // increase hue

      else if (keycode == KC_INUH)
      {
         UpdateHSV(
            user_config.hsv.h + RGB_MATRIX_HUE_STEP,  // overflow is wanted
            user_config.hsv.s,
            user_config.hsv.v
         );
         bAllowFurtherProcessing = false;
      }

      // ----------------------- SATURATION

      // decrease saturation

      else if (keycode == KC_INDS)
      {
         UpdateHSV(
            user_config.hsv.h,
            qsub8(user_config.hsv.s, RGB_MATRIX_SAT_STEP), // don't overflow
            user_config.hsv.v
         );
         bAllowFurtherProcessing = false;
      }

      // increase saturation

      else if (keycode == KC_INUS)
      {
         UpdateHSV(
            user_config.hsv.h,
            qadd8(user_config.hsv.s, RGB_MATRIX_SAT_STEP), // don't overflow
            user_config.hsv.v
         );
         bAllowFurtherProcessing = false;
      }

      // ----------------------- VALUE

      // decrease value

      else if (keycode == KC_INDV)
      {
         UpdateHSV(
            user_config.hsv.h,
            user_config.hsv.s,
            qsub8(user_config.hsv.v, RGB_MATRIX_VAL_STEP) // don't overflow
         );
         bAllowFurtherProcessing = false;
      }

      // increase value

      else if (keycode == KC_INUV)
      {
         UpdateHSV(
            user_config.hsv.h,
            user_config.hsv.s,
            qadd8(user_config.hsv.v, RGB_MATRIX_VAL_STEP) // don't overflow
         );
         bAllowFurtherProcessing = false;
      }
#endif
   }

   return bAllowFurtherProcessing;
}

// -----------------------------------------------------------------------------

void housekeeping_task_user()
{

}

bool rgb_matrix_indicators_user()
{
   if (bDipSwitchState && !host_keyboard_led_state().num_lock)
   {
#ifdef CONSOLE_ENABLE

   DPRINT(
      "rgb_matrix_indicators_user() ENABLING NUMLOCK\n"
   );
#endif
      register_code(KC_NUM_LOCK);
      unregister_code(KC_NUM_LOCK);
   }
   return true;
}

// -----------------------------------------------------------------------------
#ifdef DIP_SWITCH_ENABLE

bool dip_switch_update_user(uint8_t index, bool active)
{
#ifdef CONSOLE_ENABLE
    DPRINT(
        "dip_switch_update_user(%02X, %s)\n",
        index,
        active ? "TRUE" : "FALSE"
    );

#endif
    bDipSwitchState = active;

    return false;
}

// -----------------------------------------------------------------------------
#endif
#if defined(LED_MATRIX_ENABLE) || defined(RGB_MATRIX_ENABLE)

#define SET_LED_ON(idx) rgb_matrix_set_color(idx, rgbIndicator.r, rgbIndicator.g, rgbIndicator.b)

void os_state_indicate(void)
{
#if defined(NUM_LOCK_INDEX) || defined(CAPS_LOCK_INDEX) || defined(SCROLL_LOCK_INDEX) || defined(COMPOSE_LOCK_INDEX) || defined(KANA_LOCK_INDEX) || defined(NKRO_LOCK_INDEX)
   static RGB rgbIndicator;
   getIndicatorRGB(&rgbIndicator);
#endif
#if defined(NUM_LOCK_INDEX)

    if (!bDipSwitchState && host_keyboard_led_state().num_lock)
    {
        SET_LED_ON(NUM_LOCK_INDEX);
    }
#endif
#if defined(CAPS_LOCK_INDEX)

    if (host_keyboard_led_state().caps_lock)
    {
        SET_LED_ON(CAPS_LOCK_INDEX);
    }
#endif
#if defined(SCROLL_LOCK_INDEX)

    if (host_keyboard_led_state().scroll_lock)
    {
        SET_LED_ON(SCROLL_LOCK_INDEX);
    }
#endif
#if defined(COMPOSE_LOCK_INDEX)

    if (host_keyboard_led_state().compose)
    {
        SET_LED_ON(COMPOSE_LOCK_INDEX);
    }
#endif
#if defined(KANA_LOCK_INDEX)

    if (host_keyboard_led_state().kana)
    {
        SET_LED_ON(KANA_LOCK_INDEX);
    }
#endif
#if defined(NKRO_LOCK_INDEX)

    if (keymap_config.nkro)
    {
        SET_LED_ON(NKRO_LOCK_INDEX);
    }
#endif
}

// -----------------------------------------------------------------------------
#endif
