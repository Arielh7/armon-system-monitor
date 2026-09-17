#ifndef COLORS_H
#define COLORS_H

#define COLOR_RESET   "\033[0m"
#define COLOR_RED     "\033[31m"
#define COLOR_GREEN   "\033[32m"
#define COLOR_YELLOW  "\033[33m"
#define COLOR_CYAN    "\033[36m"
#define COLOR_BOLD    "\033[1m"

const char *color_for_percent(float percent);

const char *color_for_status(int status);

#endif