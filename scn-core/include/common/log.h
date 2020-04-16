#ifndef _COMMON_LOG_H
#define _COMMON_LOG_H

#include <stdarg.h>
#include <stdio.h>

enum log_mode {
    LOG_APPEND,
    LOG_TRUNCATE,
};

enum log_flag {
    LOG_F_TIMESTAMP = 1 << 0, /* Add a timestamp to each log entry */
    LOG_F_SYNC = 1 << 1,      /* Flush log to disk after each write */
};

struct log_handle {
    FILE *fp;
    unsigned char flags;
};

#define LOG_INIT { NULL, 0 }
#define LOG_DEFINE(name) \
	struct log_handle name = LOG_INIT;

extern struct log_handle __default_log;
#define DEFAULT_LOG (&__default_log)
extern struct log_handle __default_error_log;
#define DEFAULT_ERR_LOG (&__default_error_log)

int log_is_open(struct log_handle *lh);
void log_set_flag(struct log_handle *lh, enum log_flag flag);
void log_unset_flag(struct log_handle *lh, enum log_flag flag);
int log_open(struct log_handle *lh, const char *path, enum log_mode mode);
void log_close(struct log_handle *lh);
int log_printf(struct log_handle *lh, const char *format, ...);
int log_vprintf(struct log_handle *lh, const char *format, va_list ap);

#endif /* _COMMON_LOG_H */
