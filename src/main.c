#include "types.h"
#include "mouse.h"
#include "keypress.h"
#include "parson.h"

#include <stdio.h>

#ifdef _WIN32
    #define PRINTF printf_s
#else
    #define PRINTF printf
#endif

typedef struct KeyNames {
	const char* name;
	MMKeyCode   key;
} KeyNames;

static KeyNames key_names[] =
{
	{ "backspace",      K_BACKSPACE },
	{ "delete",         K_DELETE },
	{ "enter",          K_RETURN },
	{ "tab",            K_TAB },
	{ "escape",         K_ESCAPE },
	{ "up",             K_UP },
	{ "down",           K_DOWN },
	{ "right",          K_RIGHT },
	{ "left",           K_LEFT },
	{ "home",           K_HOME },
	{ "end",            K_END },
	{ "pageup",         K_PAGEUP },
	{ "pagedown",       K_PAGEDOWN },
	{ "f1",             K_F1 },
	{ "f2",             K_F2 },
	{ "f3",             K_F3 },
	{ "f4",             K_F4 },
	{ "f5",             K_F5 },
	{ "f6",             K_F6 },
	{ "f7",             K_F7 },
	{ "f8",             K_F8 },
	{ "f9",             K_F9 },
	{ "f10",            K_F10 },
	{ "f11",            K_F11 },
	{ "f12",            K_F12 },
	{ "f13",            K_F13 },
	{ "f14",            K_F14 },
	{ "f15",            K_F15 },
	{ "f16",            K_F16 },
	{ "f17",            K_F17 },
	{ "f18",            K_F18 },
	{ "f19",            K_F19 },
	{ "f20",            K_F20 },
	{ "f21",            K_F21 },
	{ "f22",            K_F22 },
	{ "f23",            K_F23 },
	{ "f24",            K_F24 },
	{ "capslock",       K_CAPSLOCK },
	{ "command",        K_META },
	{ "alt",            K_ALT },
	{ "right_alt",      K_RIGHT_ALT },
	{ "control",        K_CONTROL },
	{ "left_control",   K_LEFT_CONTROL },
	{ "right_control",  K_RIGHT_CONTROL },
	{ "shift",          K_SHIFT },
	{ "right_shift",    K_RIGHTSHIFT },
	{ "space",          K_SPACE },
	{ "printscreen",    K_PRINTSCREEN },
	{ "insert",         K_INSERT },
	{ "menu",           K_MENU },

	{ "audio_mute",     K_AUDIO_VOLUME_MUTE },
	{ "audio_vol_down", K_AUDIO_VOLUME_DOWN },
	{ "audio_vol_up",   K_AUDIO_VOLUME_UP },
	{ "audio_play",     K_AUDIO_PLAY },
	{ "audio_stop",     K_AUDIO_STOP },
	{ "audio_pause",    K_AUDIO_PAUSE },
	{ "audio_prev",     K_AUDIO_PREV },
	{ "audio_next",     K_AUDIO_NEXT },
	{ "audio_rewind",   K_AUDIO_REWIND },
	{ "audio_forward",  K_AUDIO_FORWARD },
	{ "audio_repeat",   K_AUDIO_REPEAT },
	{ "audio_random",   K_AUDIO_RANDOM },

	{ "numpad_lock",	K_NUMPAD_LOCK },
	{ "numpad_0",		K_NUMPAD_0 },
	{ "numpad_0",		K_NUMPAD_0 },
	{ "numpad_1",		K_NUMPAD_1 },
	{ "numpad_2",		K_NUMPAD_2 },
	{ "numpad_3",		K_NUMPAD_3 },
	{ "numpad_4",		K_NUMPAD_4 },
	{ "numpad_5",		K_NUMPAD_5 },
	{ "numpad_6",		K_NUMPAD_6 },
	{ "numpad_7",		K_NUMPAD_7 },
	{ "numpad_8",		K_NUMPAD_8 },
	{ "numpad_9",		K_NUMPAD_9 },
	{ "numpad_+",		K_NUMPAD_PLUS },
	{ "numpad_-",		K_NUMPAD_MINUS },
	{ "numpad_*",		K_NUMPAD_MULTIPLY },
	{ "numpad_/",		K_NUMPAD_DIVIDE },
	{ "numpad_.",		K_NUMPAD_DECIMAL },

	{ "lights_mon_up",    K_LIGHTS_MON_UP },
	{ "lights_mon_down",  K_LIGHTS_MON_DOWN },
	{ "lights_kbd_toggle",K_LIGHTS_KBD_TOGGLE },
	{ "lights_kbd_up",    K_LIGHTS_KBD_UP },
	{ "lights_kbd_down",  K_LIGHTS_KBD_DOWN },

	{ NULL,               K_NOT_A_KEY } /* end marker */
};

MMMouseButton convMouseButtonCode(const int code) {
	MMMouseButton button;
    if (code == 0) {
        button = LEFT_BUTTON;
    } else if (code == 1) {
        button = RIGHT_BUTTON;
    } else if (code == 2) {
        button = CENTER_BUTTON;
    } else {
        button = LEFT_BUTTON;
    }
    return button;
}

int convModifierKeyStrToKeyFlags(const char* f, MMKeyFlags* flags) {
	if (!flags) return -1;

	if (strcmp(f, "alt") == 0 || strcmp(f, "right_alt") == 0) {
		*flags = MOD_ALT;
	} else if(strcmp(f, "command") == 0) {
		*flags = MOD_META;
	} else if(strcmp(f, "control") == 0 || strcmp(f, "right_control") == 0 || strcmp(f, "left_control") == 0) {
		*flags = MOD_CONTROL;
	} else if(strcmp(f, "shift") == 0 || strcmp(f, "right_shift") == 0) {
		*flags = MOD_SHIFT;
	} else if(strcmp(f, "none") == 0) {
		*flags = MOD_NONE;
	} else {
		return -2;
	}

	return 0;
}

int checkKeyCodes(const char* k, MMKeyCode *key) {
	if (!key) return -1;

	if (strlen(k) == 1) {
		*key = keyCodeForChar(*k);
		return 0;
	}

	*key = K_NOT_A_KEY;

	KeyNames* kn = key_names;
	while (kn->name) {
		if (strcmp(k, kn->name) == 0) {
			*key = kn->key;
			break;
		}
		kn++;
	}

	if (*key == K_NOT_A_KEY) {
		return -2;
	}

	return 0;
}

void libMoveMouse(const int x, const int y) {
	MMSignedPoint point;
	point = MMSignedPointMake(x, y);
	moveMouse(point);
}

// mouseButtonCode: 0: left, 1: right, 2: center
void libMouseToggle(const bool down, const int mouseButtonCode) {
	toggleMouse(down, convMouseButtonCode(mouseButtonCode));
}

void libDragMouse(const int x, const int y, const int mouseButtonCode) {
	MMSignedPoint point;
	point = MMSignedPointMake(x, y);
	dragMouse(point, convMouseButtonCode(mouseButtonCode));
}

void libMouseClick(const bool isDoubleClick, const int mouseButtonCode) {
	MMMouseButton button = convMouseButtonCode(mouseButtonCode);
	if (!isDoubleClick) {
		clickMouse(button);
	} else {
		doubleClick(button);
	}
}

void libScrollMouse(const int x, const int y) {
	scrollMouse(x, y);
}

int libKeyToggle(const char* keyCodeStr, const char* directionStr, const char** modifierKeys, const int modifierKeysLength) {
	MMKeyFlags flags = MOD_NONE;
	MMKeyCode key;

    // Get key modifier
	for (int i=0; i<modifierKeysLength; ++i) {
		MMKeyFlags flag = MOD_NONE;
		const char* modifierKey = modifierKeys[i];
		PRINTF("calling convModifierKeyStrToKeyFlags with modifierKey=%s\n", modifierKey);
		const int errFlag = convModifierKeyStrToKeyFlags(modifierKey, &flag);
		PRINTF("convModifierKeyStrToKeyFlags returned errFlag=%d\n", errFlag);
		if (errFlag < 0) {
			return -1;
		}

		flags = (MMKeyFlags)(flags | flag);
	}

	// Get the actual key
	PRINTF("calling checkKeyCodes() with keyCodeStr==%s\n", keyCodeStr);
	const int errKey = checkKeyCodes(keyCodeStr, &key);
	PRINTF("checkKeyCodes() returned with errKey=%d key=%d\n", errKey, key);
    if (errKey < 0) {
        return -2;
    }

	bool isDown = true;
	if (strcmp(directionStr, "up") == 0) {
		isDown = false;
	}

	toggleKeyCode(key, isDown, flags);

	return 0;
}

int main(int argc, char* argv[]) {

	if (argc == 3) {
		int posX = atoi(argv[1]);
		int posY = atoi(argv[2]);

		libMoveMouse(posX, posY);
	}

/*
	const char* inputStr = "{\"op\":\"keyToggle\", \"keyCode\":\"b\", \"direction\":\"down\", \"modifiers\":[\"shift\"]}";

	JSON_Value* root_value = json_parse_string(inputStr);
    JSON_Object* root_object = json_value_get_object(root_value);

	const char* op = json_object_get_string(root_object, "op");
	if (strcmp(op, "keyToggle") == 0) {
		const char* keyCodeStr = json_object_get_string(root_object, "keyCode");
		const char* directionStr = json_object_get_string(root_object, "direction");

		printf("keyCodeStr=%s\n", keyCodeStr);
		printf("directionStr=%s\n", directionStr);

		char* modifiers[8] = {0};
		JSON_Array* json_modifiers = json_object_get_array(root_object, "modifiers");
		int modifiersLength = json_array_get_count(json_modifiers);
		for (int i=0; i<modifiersLength; ++i) {
			modifiers[i] = json_array_get_string(json_modifiers, i);
			printf("modifiers[%d]=%s\n", i, modifiers[i]);
		}

		int s = libKeyToggle(keyCodeStr, directionStr, modifiers, modifiersLength);
		printf("libKeyToggle returned %d\n", s);
	}

	json_value_free(root_value);
*/

    return 0;
}
