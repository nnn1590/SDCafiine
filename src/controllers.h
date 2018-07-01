#include <vpad/input.h>
#include <padscore/wpad.h>
#include <padscore/kpad.h>

enum PadButton {
	PAD_BTN_A,
	PAD_BTN_UP,
	PAD_BTN_DOWN,
	PAD_BTN_L,
	PAD_BTN_R
};

void initControllers();
void releaseControllers();
void pollControllers();
bool isButtonPressed(enum PadButton btn);
