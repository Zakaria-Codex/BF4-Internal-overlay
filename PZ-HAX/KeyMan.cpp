#include "KeyMan.h"

bool KeyMan::ReadKeyOnce(unsigned int keycode, unsigned int timer) {
	if (KeyMan::requireActive && !KeyMan::windowActive) return false;
	if ((clock() - lastTime > timer) && GetAsyncKeyState(keycode)) {
		KeyMan::lastTime = clock();
		return true;
	}
	return false;
}

bool KeyMan::ReadKeyOnce(KeyDef keyDef, unsigned int timer) {
	return KeyMan::ReadKeyOnce(keyDef.keyCode, timer);
}

bool KeyMan::ReadKey(unsigned int keycode) {
	if (KeyMan::requireActive && !KeyMan::windowActive) return false;
	return GetAsyncKeyState(keycode);
}

bool KeyMan::ReadKey(KeyDef keyDef) {
	return KeyMan::ReadKey(keyDef.keyCode);
}


bool KeyMan::windowActive = true;
bool KeyMan::requireActive = true;

time_t KeyMan::lastTime = clock();

KeyDef Keys::showMenuKey = { "HOME", VK_HOME };
KeyDef Keys::closeCheatKey = { "END", VK_END };

