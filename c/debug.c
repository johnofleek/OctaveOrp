#include <stdarg.h>
#include <stdio.h>

#include "debug.h"

#if defined(ARDUINO_ARCH_AVR)
void db_print(const char *fmt, ...)
{
     (void)fmt;     // work around compiler warning
}

#else
void db_print(const char *fmt, ...)
{
    va_list args;
    va_start(args, fmt);
    vfprintf(stderr, fmt, args);
    va_end(args);
}
#endif