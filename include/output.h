#ifndef OUTPUT_H
#define OUTPUT_H

#include <stddef.h>

void out_begin(void);

void out_printf(const char *fmt, ...);

void out_flush(void);

#endif