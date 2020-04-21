/*
Copyright <2020> <J Thompson>

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:
The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.
THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/

#ifndef DEBUG_H
#define DEBUG_H

#ifdef __cplusplus
extern "C" {
#endif


/* Control whether debugging macros are active at compile time */
#undef DB_ACTIVE
#ifdef DEBUG
#define DB_ACTIVE 1
#else
#define DB_ACTIVE 0
#endif /* DEBUG */

void     db_print(const char *fmt, ...);

/*
** Usage:  TRACE((level, fmt, ...))
** "level" is the debugging level which must be operational for the output
** to appear. "fmt" is a printf format string. "..." is whatever extra
** arguments fmt requires (possibly nothing).
** The non-debug macro means that the code is validated but never called.
** -- See chapter 8 of 'The Practice of Programming', by Kernighan and Pike.
*/


#define TRACE(x) \
            do { if (DB_ACTIVE) db_print x; } while (0)


#ifdef __cplusplus
}
#endif

#endif /* DEBUG_H */