#ifndef _COMMON_DEBUG_H
#define _COMMON_DEBUG_H

#include <stdio.h>
#include "log.h"
#include "platform.h"

#if defined(OS_ANDROID)
#include <android/log.h>
#if defined(ENABLE_DEBUG)
#define LOG_DBG(format, ...) __android_log_print(ANDROID_LOG_DEBUG, "Serval", \
                                                 "%s: "format, __func__, ## __VA_ARGS__)
#else
#define LOG_DBG(format, ...)
#endif /* ENABLE_DEBUG */
#define LOG_ERR(format, ...) __android_log_print(ANDROID_LOG_ERROR, "Serval", "%s: ERROR "format, \
                                                 __func__, ## __VA_ARGS__)
#else
#if defined(ENABLE_DEBUG)
#define LOG_DBG(format, ...) ({                                         \
            log_printf(DEFAULT_LOG, "%s: "format, __func__, ## __VA_ARGS__); \
        })
#else
#define LOG_DBG(format, ...)
#endif
#define LOG_ERR(format, ...) ({                                         \
            log_printf(DEFAULT_ERR_LOG, "%s: "format, __func__, ## __VA_ARGS__); \
        })
#endif /* OS_ANDROID */

#endif /* _COMMON_DEBUG_H */
