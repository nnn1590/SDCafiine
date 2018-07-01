#include <string>

#include <map>
#include <string.h>
#include <stdlib.h>
#include <stdarg.h>
#include <stdio.h>
#include <malloc.h>
#include "modpackSelector.h"
#include "common/common.h"
#include "controllers.h"
#include "version.h"

#include <coreinit/screen.h>
#include <coreinit/time.h>
#include <coreinit/thread.h>
#include <utils/logger.h>
#include <utils/StringTools.h>
#include <fs/FSUtils.h>
#include <fs/CFile.hpp>
#include <fs/DirList.h>
#include "common/retain_vars.h"

/* FS defines and types */
#define FS_MAX_LOCALPATH_SIZE           511
#define FS_MAX_MOUNTPATH_SIZE           128
#define FS_MAX_FULLPATH_SIZE            (FS_MAX_LOCALPATH_SIZE + FS_MAX_MOUNTPATH_SIZE)
#define FS_MAX_ARGPATH_SIZE FS_MAX_FULLPATH_SIZE
#define FS_MAX_ENTNAME_SIZE 256

#define TEXT_SEL(x, text1, text2)           ((x) ? (text1) : (text2))

void HandleMultiModPacks(uint64_t titleID/*,bool showMenu*/) {
    gModFolder[0] = 0;

    char TitleIDString[FS_MAX_FULLPATH_SIZE];
    snprintf(TitleIDString,FS_MAX_FULLPATH_SIZE,"%016llX",titleID);

    std::map<std::string,std::string> modTitlePath;

    std::map<std::string,std::string> mounting_points;
    if(gSDInitDone) {
        mounting_points[std::string(SD_PATH)] = std::string(NAME_PREFIX_SD);
    }
    if(gUSBInitDone) {
        mounting_points[std::string(USB_PATH)] = std::string(NAME_PREFIX_USB);
    }
    //int32_t i = 0;

    //for (i = 0; i < ntfs_mount_count; i++){
    //mounting_points[std::string(((ntfs_md *)ntfs_mounts)[i].name) + ":"] = StringTools::strfmt("%s:(%s)",((ntfs_md *)ntfs_mounts)[i].name, ntfsGetVolumeName(((ntfs_md *)ntfs_mounts)[i].name));
    //}

    for (std::map<std::string,std::string>::iterator it=mounting_points.begin(); it!=mounting_points.end(); ++it) {

        std::string curMount = it->first;
        std::string curMountName = it->second;
        DEBUG_FUNCTION_LINE("%s %s \n",curMount.c_str(),curMountName.c_str());
        std::string modTitleIDPath = curMount + GAME_MOD_FOLDER + "/" + TitleIDString;
        DEBUG_FUNCTION_LINE("modTitleIDPath %s \n",modTitleIDPath.c_str());
        DirList modTitleDirList(modTitleIDPath.c_str(), NULL, DirList::Dirs);

        modTitleDirList.SortList();

        for(int32_t i = 0; i < modTitleDirList.GetFilecount(); i++) {
            std::string curFile = modTitleDirList.GetFilename(i);
            //DEBUG_FUNCTION_LINE("curFile %s \n",curFile.c_str());
            if(curFile.compare(".") == 0 || curFile.compare("..") == 0)
                continue;

            if(curFile.compare(CONTENT_FOLDER) == 0 || curFile.compare(AOC_FOLDER) == 0/* || curFile.compare(META_FOLDER) == 0*/) {
                std::string packageName = curMountName + " " + DEFAULT_NAME_PACKAGE;
                modTitlePath[packageName] = modTitleIDPath;
                DEBUG_FUNCTION_LINE("found %s \n",packageName.c_str());
            } else {
                std::string packageName = curMountName + " " + curFile;
                modTitlePath[packageName] = modTitleIDPath + "/" + curFile;
                DEBUG_FUNCTION_LINE("found %s \n",packageName.c_str());
            }
        }
    }

    DEBUG_FUNCTION_LINE("Iteration done\n");

    int32_t modPackListSize =modTitlePath.size();

    if(modPackListSize == 0)
        return;
    if(modPackListSize == 1/* || !showMenu*/) {
        for (std::map<std::string,std::string>::iterator it=modTitlePath.begin(); it!=modTitlePath.end(); ++it) {
            snprintf(gModFolder, FS_MAX_ENTNAME_SIZE, "%s", it->second.c_str());
            break;
        }
        return;
    }

    int32_t selected = 0;
    int32_t initScreen = 1;
    int32_t x_offset = -2;

    initControllers();

    OSScreenInit();
    uint32_t screen_buf0_size = OSScreenGetBufferSizeEx(SCREEN_TV);
    uint32_t screen_buf1_size = OSScreenGetBufferSizeEx(SCREEN_DRC);
    uint32_t * screenbuffers = (uint32_t*)memalign(0x100, screen_buf0_size + screen_buf1_size);
    OSScreenSetBufferEx(SCREEN_TV, (void *)screenbuffers);
    OSScreenSetBufferEx(SCREEN_DRC, (void *)(screenbuffers + screen_buf0_size));

    OSScreenEnableEx(SCREEN_TV, 1);
    OSScreenEnableEx(SCREEN_DRC, 1);

    // Clear screens
    OSScreenClearBufferEx(SCREEN_TV, 0);
    OSScreenClearBufferEx(SCREEN_DRC, 0);

    // Flip buffers
    OSScreenFlipBuffersEx(SCREEN_TV);
    OSScreenFlipBuffersEx(SCREEN_DRC);

    int32_t wantToExit = 0;
    int32_t page = 0;
    int32_t per_page = 13;
    int32_t max_pages = (modPackListSize / per_page) + 1;

    while(1) {

        pollControllers();

        if (isButtonPressed(PAD_BTN_A)) {
            wantToExit = 1;
            initScreen = 1;
        } else if (isButtonPressed(PAD_BTN_DOWN)) {
            selected++;
            initScreen = 1;
        } else if (isButtonPressed(PAD_BTN_UP)) {
            selected--;
            initScreen = 1;
        } else if (isButtonPressed(PAD_BTN_L)) {
            selected -= per_page;
            initScreen = 1;
        } else if (isButtonPressed(PAD_BTN_R)) {
            selected += per_page;
            initScreen = 1;
        }

        if (selected < 0)
            selected = 0;
        if (selected >= modPackListSize)
            selected = modPackListSize - 1;
        page = selected / per_page;

        if(initScreen) {
            OSScreenClearBufferEx(SCREEN_TV, 0);
            OSScreenClearBufferEx(SCREEN_DRC, 0);
            std::string text = StringTools::strfmt("                  -- SDCafiine %s by Maschell --",APP_VERSION);
            console_print_pos(x_offset, -1, text.c_str());
            console_print_pos(x_offset, 1, "Select your options and press A to launch.");
            int32_t y_offset = 3;
            int32_t cur_ = 0;

            for (std::map<std::string,std::string>::iterator it=modTitlePath.begin(); it!=modTitlePath.end(); ++it) {
                std::string key = it->first;
                std::string value = it->second;

                if(wantToExit && cur_ == selected) {
                    snprintf(gModFolder, FS_MAX_ENTNAME_SIZE, "%s", value.c_str());
                    break;
                }

                if(cur_ >= (page*per_page) && cur_ < ((page+1)*per_page)) {
                    std::string text = StringTools::strfmt("%s %s",TEXT_SEL((selected == cur_), "--->", "    "), key.c_str());
                    console_print_pos(x_offset, y_offset++, text.c_str());
                }
                cur_++;
            }

            if(wantToExit) { //just in case.
                break;
            }

            if(max_pages > 0) {
                std::string text = StringTools::strfmt("Page %02d/%02d. Press L/R to change page.", page + 1,max_pages);
                console_print_pos(x_offset, 17, text.c_str());
            }

            // Flip buffers
            OSScreenFlipBuffersEx(SCREEN_TV);
            OSScreenFlipBuffersEx(SCREEN_DRC);

            initScreen = 0;
        }
        OSSleepTicks(OSMicrosecondsToTicks(20000));
    }

    OSScreenClearBufferEx(SCREEN_TV, 0);
    OSScreenClearBufferEx(SCREEN_DRC, 0);

    // Flip buffers
    OSScreenFlipBuffersEx(SCREEN_TV);
    OSScreenFlipBuffersEx(SCREEN_DRC);

    releaseControllers();

    free(screenbuffers);

    return;
}

void console_print_pos(int32_t x, int32_t y, const char *text) {
    if(text == NULL) {
        return;
    }
    OSScreenPutFontEx(SCREEN_TV, x, y, text);
    OSScreenPutFontEx(SCREEN_DRC, x, y, text);
}
