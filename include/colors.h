#ifndef COLORS_H
#define COLORS_H


#define COLOR_RESET   "\033[0m"
#define COLOR_RED     "\033[31m"
#define COLOR_GREEN   "\033[32m"
#define COLOR_YELLOW  "\033[33m"
#define COLOR_BLUE    "\033[34m"
#define COLOR_MAGENTA "\033[35m"
#define COLOR_CYAN    "\033[36m"
#define COLOR_WHITE   "\033[97m"
#define COLOR_LABEL   "\033[97m"


#define COLOR_CHROME  "\033[97m"   


#define COLOR_BOLD    "\033[1m"
#define COLOR_DIM     "\033[2m"
#define COLOR_UNDER   "\033[4m"
#define COLOR_REVERSE "\033[7m"

#define BG_GRAY       "\033[100m"
#define BG_WHITE      "\033[47m"
#define BG_BLACK      "\033[40m"

const char *color_for_percent(float percent);

#endif