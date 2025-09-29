#pragma once
#include <stdarg.h>

#ifdef __cplusplus
extern "C"
{
#endif

    typedef enum
    {
        METRO_OK = 0,        /* success */
        METRO_EINVAL = -1,   /* bad arguments */
        METRO_ENOMEM = -2,   /* allocation failed */
        METRO_ENOENT = -3,   /* file/row not found */
        METRO_EIO = -4,      /* fopen/fseek/ftell/fprintf/fread error */
        METRO_EPARSE = -5,   /* malformed TSV line */
        METRO_EDUP = -6,     /* duplicate (e.g., order slot taken) */
        METRO_EOVERFLOW = -7 /* value doesn’t fit (e.g., > 0xFFFF) */
    } metro_err_t;

    void metro_set_errorf(const char *fmt, ...);
    const char *metro_last_error_msg(void);

    const char *metro_err_name(metro_err_t code);

#ifdef __cplusplus
}
#endif
