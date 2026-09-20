#include <stdio.h>
#include <stdarg.h>
#include "output.h"

#define OUT_BUFFER_SIZE 65536

static char   buffer[OUT_BUFFER_SIZE];
static size_t length = 0;

void out_begin(void) {
    length = 0;
}

void out_printf(const char *fmt, ...) {
    if (length >= OUT_BUFFER_SIZE - 1) return;

    va_list args;
    va_start(args, fmt);
    int written = vsnprintf(buffer + length, OUT_BUFFER_SIZE - length, fmt, args);
    va_end(args);

    if (written > 0) {
        length += (size_t)written;
        if (length >= OUT_BUFFER_SIZE) length = OUT_BUFFER_SIZE - 1;
    }
}

void out_flush(void) {
    if (length == 0) return;
    fwrite(buffer, 1, length, stdout);
    fflush(stdout);
    length = 0;
}