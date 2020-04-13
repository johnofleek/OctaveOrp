#include <stdarg.h>
#include <stdio.h>

#include "debug.h"

void db_print(const char *fmt, ...)
{
    va_list args;
    va_start(args, fmt);
    vfprintf(stderr, fmt, args);
    va_end(args);
}