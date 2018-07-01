#include <utils/logger.h>
#include "controllers.h"

VPADStatus vpad;
VPADReadError vpadError;

KPADStatus pads[4];
int32_t padErrors[4];
WPADExtensionType padTypes[4];

void initControllers() {
	KPADInit();
}

void releaseControllers() {

    if((padErrors[0] = WPADProbe(WPAD_CHAN_0, &padTypes[0])) == 0){
        WPADDisconnect(WPAD_CHAN_0);
	}

    if((padErrors[1] = WPADProbe(WPAD_CHAN_1, &padTypes[1])) == 0){
        WPADDisconnect(WPAD_CHAN_1);
	}

    if((padErrors[2] = WPADProbe(WPAD_CHAN_2, &padTypes[2])) == 0){
        WPADDisconnect(WPAD_CHAN_2);
	}

    if((padErrors[3] = WPADProbe(WPAD_CHAN_3, &padTypes[3])) == 0){
        WPADDisconnect(WPAD_CHAN_3);
	}


	//Clear buffers so future launches of SDCafiine don't read old data
	memset(&vpad, 0, sizeof(VPADStatus));
	vpadError = VPAD_READ_SUCCESS;

	memset(pads, 0, sizeof(KPADStatus) * 4);
	memset(padErrors, 0, sizeof(int32_t) * 4);
	memset(padTypes, 0, sizeof(uint32_t) * 4);
}

void pollControllersSingle(WPADChan chan) {
    if((padErrors[chan] = WPADProbe(chan, &padTypes[chan])) == 0) {
        KPADRead(chan, &pads[chan], 1);
    }
}
void pollControllers() {
	VPADRead(VPAD_CHAN_0, &vpad, 1, &vpadError);

    pollControllersSingle(WPAD_CHAN_0);
    pollControllersSingle(WPAD_CHAN_1);
    pollControllersSingle(WPAD_CHAN_2);
    pollControllersSingle(WPAD_CHAN_3);
}

bool isButtonPressed(enum PadButton btn) {
	if(vpadError == VPAD_READ_SUCCESS) {
		switch (btn) {
		case PAD_BTN_A:
			if (vpad.trigger & VPAD_BUTTON_A) return true;
			break;

		case PAD_BTN_UP:
			if (vpad.trigger & VPAD_BUTTON_UP) return true;
			break;

		case PAD_BTN_DOWN:
			if (vpad.trigger & VPAD_BUTTON_DOWN) return true;
			break;

		case PAD_BTN_L:
			if (vpad.trigger & VPAD_BUTTON_L) return true;
			break;

		case PAD_BTN_R:
			if (vpad.trigger & VPAD_BUTTON_R) return true;
			break;

		default:
			break;
		}
	}

	for(int32_t i = 0; i < 4; i++) {
		if(padErrors[i] == 0) {
			if(pads[i].extensionType < 2) {
				switch(btn) {
				case PAD_BTN_A:
					if (pads[i].trigger & WPAD_BUTTON_A) return true;
					break;

				case PAD_BTN_UP:
					if (pads[i].trigger & WPAD_BUTTON_UP) return true;
					break;

				case PAD_BTN_DOWN:
					if (pads[i].trigger & WPAD_BUTTON_DOWN) return true;
					break;

				case PAD_BTN_L:
					if (pads[i].trigger & WPAD_BUTTON_1) return true;
					break;

				case PAD_BTN_R:
					if (pads[i].trigger & WPAD_BUTTON_2) return true;
					break;

				default:
					break;
				}
			}
			else {
				switch(btn) {
				case PAD_BTN_A:
					if (pads[i].classic.trigger & WPAD_CLASSIC_BUTTON_A) return true;
					break;

				case PAD_BTN_UP:
					if (pads[i].classic.trigger & WPAD_CLASSIC_BUTTON_UP) return true;
					break;

				case PAD_BTN_DOWN:
					if (pads[i].classic.trigger & WPAD_CLASSIC_BUTTON_DOWN) return true;
					break;

				case PAD_BTN_L:
					if (pads[i].classic.trigger & WPAD_CLASSIC_BUTTON_L) return true;
					break;

				case PAD_BTN_R:
					if (pads[i].classic.trigger & WPAD_CLASSIC_BUTTON_R) return true;
					break;

				default:
					break;
				}
			}
		}
	}

	return false;
}
