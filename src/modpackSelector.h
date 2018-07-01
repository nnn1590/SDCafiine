#ifndef _MODPACK_SELECTOR_H_
#define _MODPACK_SELECTOR_H_

#ifdef __cplusplus
extern "C" {
#endif
#include <wut_types.h>

void HandleMultiModPacks(uint64_t titleid/*,bool showMenu = true*/);
void console_print_pos(int32_t x, int32_t y, const char *text);

#ifdef __cplusplus
}
#endif

#endif //_MODPACK_SELECTOR_H_
