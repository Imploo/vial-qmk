#include QMK_KEYBOARD_H
#define ____ KC_TRNS

#ifdef AUTO_SHIFT_ENABLE

void keyboard_post_init_user(void) {
	autoshift_disable();
}
#endif

bool is_macOS = true;
bool is_alt_tab_active = false;
bool is_ctl_tab_active = false;
uint16_t alt_tab_timer = 0;

enum custom_keycodes {
  ALT_TAB = QK_KB_0,
  ALT_TABR,
  CTL_TAB,
  CTL_TABR
};

void update_swap_led(void) {
    if (keymap_config.swap_lctl_lgui == is_macOS) {
        rgblight_set_effect_range(1, 90);
        rgblight_sethsv_at(0, 255, rgblight_get_val(), 0);
    } else {
        rgblight_set_effect_range(0, 90);
    }
}

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
  switch (keycode) {
    case CG_TOGG:
        update_swap_led();
        break;
    case ALT_TAB:
        if (record->event.pressed) {
            if (!is_alt_tab_active) {
                is_alt_tab_active = true;
                register_code(is_macOS ? KC_LGUI : KC_LALT);
            }
            alt_tab_timer = timer_read();
            register_code(KC_TAB);
        } else {
            unregister_code(KC_TAB);
        }
        break;
    case ALT_TABR:
        if (record->event.pressed) {
            if (!is_alt_tab_active) {
                is_alt_tab_active = true;
                register_code(is_macOS ? KC_LGUI : KC_LALT);
            }
            alt_tab_timer = timer_read();
            register_code(KC_LSFT);
            register_code(KC_TAB);
        } else {
            unregister_code(KC_TAB);
            unregister_code(KC_LSFT);
        }
        break;
    case CTL_TAB:
        if (record->event.pressed) {
            if (!is_ctl_tab_active) {
                is_ctl_tab_active = true;
                register_code(KC_LCTL);
            }
            alt_tab_timer = timer_read();
            register_code(KC_TAB);
        } else {
            unregister_code(KC_TAB);
        }
        break;
    case CTL_TABR:
        if (record->event.pressed) {
            if (!is_ctl_tab_active) {
                is_ctl_tab_active = true;
                register_code(KC_LCTL);
            }
            alt_tab_timer = timer_read();
            register_code(KC_LSFT);
            register_code(KC_TAB);
        } else {
            unregister_code(KC_TAB);
            unregister_code(KC_LSFT);
        }
        break;
  }
  return true;
}

void matrix_scan_user(void) { // The very important timer.
  if (is_alt_tab_active) {
    if (timer_elapsed(alt_tab_timer) > 1000) {
      unregister_code(is_macOS ? KC_LGUI : KC_LALT);
      is_alt_tab_active = false;
    }
  }
  if (is_ctl_tab_active) {
    if (timer_elapsed(alt_tab_timer) > 1000) {
      unregister_code(KC_LCTL);
      is_ctl_tab_active = false;
    }
  }
}

bool process_detected_host_os_user(os_variant_t detected_os) {
    is_macOS = detected_os > 2;
    if (!is_macOS) {
        keymap_config.swap_lctl_lgui = true;
    }
    update_swap_led();
    return true;
}


// Function to set the color for different layers
layer_state_t layer_state_set_user(layer_state_t state) {
    switch (get_highest_layer(state)) {
        case 0: // Base layer
            rgblight_mode(RGBLIGHT_MODE_RAINBOW_SWIRL); // Set rainbow mode
            break;
        case 1: // Function layer
            rgblight_mode(RGBLIGHT_MODE_STATIC_LIGHT);
            rgblight_sethsv_noeeprom(120, 255, rgblight_get_val());
            break;
        default:
            rgblight_setrgb(RGB_WHITE); // Default to white if unknown layer
            break;
    }
    return state;
}

#define SFT_ENT SFT_T(KC_ENTER)
#define SFT_BSPC SFT_T(KC_BSPC)
#define GUI_CAPS GUI_T(KC_CAPS)

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {

    [0] = LAYOUT(
		QK_GESC,  KC_F1,   KC_F2,   KC_F3,   KC_F4,   KC_F5, KC_7,     KC_8,  KC_9,     KC_F6, KC_F7,   KC_F8,    KC_F9, KC_F10, KC_F11,  KC_F12,  KC_MINS, KC_EQL,
		KC_TAB,   KC_Q,    KC_W,    KC_E,    KC_R,    KC_T,  KC_4,     KC_5,  KC_6,     KC_NO, KC_LBRC, KC_RBRC,  KC_Y,  KC_U,   KC_I,    KC_O,    KC_P,    KC_BSLS,
		GUI_CAPS, KC_A,    KC_S,    KC_D,    KC_F,    KC_G,  KC_1,     KC_2,  KC_3,     KC_NO, KC_LPRN, KC_RPRN,  KC_H,  KC_J,   KC_K,    KC_L,    KC_SCLN, KC_QUOT,
		KC_LSFT,  KC_Z,    KC_X,    KC_C,    KC_V,    KC_B,  SFT_ENT,  KC_0,                   KC_DEL,  SFT_BSPC, KC_N,  KC_M,   KC_COMM, KC_DOT,  KC_UP,   KC_SLSH,
		KC_LCTL,  KC_LALT, KC_LGUI, G(KC_C), G(KC_V),        KC_SPACE,                                  KC_SPACE,        TT(1),  KC_NO,   KC_LEFT, KC_DOWN, KC_RIGHT
	),

	[1] = LAYOUT(
		____, ____, ____, ____, ____, ____, ____, ____, ____,   ____, ____, ____, ____, ____, ____, ____, ____, ____,
		____, ____, ____, ____, ____, ____, ____, ____, ____,   ____, ____, ____, ____, ____, ____, ____, ____, ____,
		____, ____, ____, ____, ____, ____, ____, ____, ____,   ____, ____, ____, ____, ____, ____, ____, ____, ____,
		____, ____, ____, ____, ____, ____, ____, ____,               ____, ____, ____, ____, ____, ____, ____, ____,
		____, ____, ____, ____, ____,       ____,                           ____,       ____, ____, ____, ____, ____
	)
};

#if defined(ENCODER_MAP_ENABLE)
const uint16_t PROGMEM encoder_map[][NUM_ENCODERS][2] = {
    [0] = { ENCODER_CCW_CW(ALT_TAB, ALT_TABR), ENCODER_CCW_CW(KC_UNDO, LSG(KC_Z)) },
    [1] = { ENCODER_CCW_CW(CTL_TAB, CTL_TABR), ENCODER_CCW_CW(KC_TRNS, KC_TRNS) }
};
#endif

#ifdef OLED_ENABLE

static void print_status_narrow(void) {
	// Create OLED content
    oled_advance_page(true);
    oled_write_P(PSTR("Lotus -58-"), false);
    oled_advance_page(true);

    // Print current layer
    oled_write_P(PSTR("Layer"), false);
    switch (get_highest_layer(layer_state)) {
        case 0:
            oled_write_P(PSTR("-Base\n"), false);
            break;
        case 1:
            oled_write_P(PSTR("-Func\n"), false);
            break;
        case 2:
            oled_write_P(PSTR("-Num \n"), false);
            break;
        case 3:
            oled_write_P(PSTR("-Sys \n"), false);
            break;
        default:
            oled_write_P(PSTR("Undef"), false);
    }

	// Display capslock
    oled_advance_page(true);
    led_t led_usb_state = host_keyboard_led_state();
    oled_write_ln_P(PSTR("Caps- lock"), led_usb_state.caps_lock);

#ifdef AUTO_SHIFT_ENABLE

	bool autoshift = get_autoshift_state();
	oled_advance_page(true);
	oled_write_P(PSTR("Auto-Shift"), autoshift);
	oled_advance_page(true);

#endif

}

bool oled_task_user(void) {
    print_status_narrow();
	return false;
}


#endif
