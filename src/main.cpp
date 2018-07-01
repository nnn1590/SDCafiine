#include <wups.h>
#include <string.h>

#include <utils/logger.h>
#include <nsysnet/socket.h>
#include <coreinit/title.h>
#include "common/retain_vars.h"
#include <fswrapper/fs_retain_vars.h>
#include <fswrapper/FileReplacerUtils.h>
#include "common/common.h"
#include "main.h"
#include "modpackSelector.h"
#include "version.h"

WUPS_PLUGIN_NAME("SDCaffiine");
WUPS_PLUGIN_DESCRIPTION("Replaces the game files on the fly. Can be used for gamemods");
WUPS_PLUGIN_VERSION(APP_VERSION);
WUPS_PLUGIN_AUTHOR("Maschell");
WUPS_PLUGIN_LICENSE("GPL");

WUPS_FS_ACCESS()

ON_APPLICATION_START(args) {
    socket_lib_init();

    log_init();

    if(FileReplacerUtils::setGroupAndOwnerID()) {
        //DEBUG_FUNCTION_LINE("SUCCESS\n");
    }

    DEBUG_FUNCTION_LINE("SD mounted? %d\n",args.sd_mounted);
    DEBUG_FUNCTION_LINE("USB mounted? %d\n",args.usb_mounted);

	gSDInitDone = args.sd_mounted;
	gUSBInitDone = args.usb_mounted;

    HandleMultiModPacks(OSGetTitleID());

    log_print("Init of SDCafiine!\n");
}

ON_APPLICATION_ENDING() {
    FileReplacerUtils::destroyInstance();
}

ON_APP_STATUS_CHANGED(status) {
	gAppStatus = status;
}
