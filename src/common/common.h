#ifndef COMMON_H
#define    COMMON_H

#ifdef __cplusplus
extern "C" {
#endif

#define CAFE_OS_SD_PATH             "/vol/external01"
#define GAME_MOD_FOLDER             "/sdcafiine"
#define SD_PATH                     "sd:"
#define USB_PATH                    "usb:"
#define DEFAULT_NAME_PACKAGE        "DEFAULT"
#define NAME_PREFIX_SD              "sd: "
#define NAME_PREFIX_USB             "usb:"
#define WIIU_PATH                   "/wiiu"
#define IP_TXT                      "ip.txt"

#define CONTENT_FOLDER              "content"
#define AOC_FOLDER                  "aoc"
#define META_FOLDER                 "meta"

#define BOOT_TV_TEX_TGA             "bootTvTex.tga"
#define BOOT_DRC_TEX_TGA            "bootDrcTex.tga"
#define BOOT_SOUND_BTSND            "bootSound.btsnd"

#define GAME_PATH_TYPE_CONTENT              1
#define GAME_PATH_TYPE_AOC                  2
#define GAME_PATH_TYPE_STRIPPED_CONTENT     3

#define SDUSB_MOUNTED_NONE          0
#define SDUSB_MOUNTED_FAKE          (1<<0)
#define SDUSB_MOUNTED_OS_SD         (1<<1)
#define SDUSB_LIBIOSU_LOADED        (1<<2)
#define SD_MOUNTED_LIBFAT           (1<<3)
#define USB_MOUNTED_LIBFAT          (1<<4)
#define USB_MOUNTED_LIBNTFS         (1<<5)

#define FILELIST_NAME               "filelist.txt"
#define DIR_IDENTIFY                "?"  /* maximum length = 1*/
#define PARENT_DIR_IDENTIFY         "?.."

#ifdef __cplusplus
}
#endif

#endif    /* COMMON_H */

