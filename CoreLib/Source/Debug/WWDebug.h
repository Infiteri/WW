#pragma once

#define WW_DEBUG 1

#if WW_DEBUG == 1
#include <stdio.h>
#define WW_LOG(msg, ...) printf("[WW DEBUG] " msg "\n", ##__VA_ARGS__);
#define WW_LOG_LVL(lvl, msg, ...) printf("[WW %s] " msg "\n", lvl, ##__VA_ARGS__);
#else
#define WW_LOG(msg, ...)
#define WW_LOG_LVL(msg, ...)
#endif