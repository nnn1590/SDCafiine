#ifndef RETAINS_VARS_H_
#define RETAINS_VARS_H_
#include <coreinit/filesystem.h>

#define ASYNC_RESULT_CACHE_SIZE     50
#define FS_QUEUE_MESSAGE_COUNT      5

extern uint8_t gAppStatus;
extern volatile bool gSDInitDone;
extern volatile bool gUSBInitDone;

extern char gModFolder[256];

#endif // RETAINS_VARS_H_
