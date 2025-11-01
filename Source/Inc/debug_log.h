#ifndef DEBUG_LOG_H
#define DEBUG_LOG_H

#include <stdio.h>

typedef enum {
	LOG_LEVEL_NONE=0,
	LOG_LEVEL_ERROR,
	LOG_LEVEL_WARN,
	LOG_LEVEL_INFO,
	LOG_LEVEL_DEBUG,
} LogLevel;

/*
 * Compile-time global log level
 * 可改成在 Makefile 定義，例如：
 *   CFLAGS += -DGLOBAL_LOG_LEVEL=LOG_LEVEL_INFO
 */
#ifndef GLOBAL_LOG_LEVEL
#define GLOBAL_LOG_LEVEL LOG_LEVEL_DEBUG
#endif

#define LOG_PRINT(level_str, module, fmt, ...)							\
	do{																	\
		printf("[%s][%s]" fmt "\n", level_str, module, ##__VA_ARGS__);	\
	}while(0)

#define LOGE(fmt, ...)													\
	do {																\
		if (GLOBAL_LOG_LEVEL >= LOG_LEVEL_ERROR)						\
			LOG_PRINT("ERR", LOG_MODULE, fmt, ##__VA_ARGS__);			\
    } while (0)

#define LOGW(fmt, ...)													\
	do {																\
		if (GLOBAL_LOG_LEVEL >= LOG_LEVEL_WARN)							\
			LOG_PRINT("WRN", LOG_MODULE, fmt, ##__VA_ARGS__)			\
	} while (0)

#define LOGI(fmt, ...)													\
	do {																\
		if (GLOBAL_LOG_LEVEL >= LOG_LEVEL_INFO)							\
			LOG_PRINT("INF", LOG_MODULE, fmt, ##__VA_ARGS__);			\
	} while (0)

#define LOGD(fmt, ...)													\
	do {																\
		if (GLOBAL_LOG_LEVEL >= LOG_LEVEL_DEBUG)						\
			LOG_PRINT("DBG", LOG_MODULE, fmt, ##__VA_ARGS__);			\
	} while (0)

#endif

