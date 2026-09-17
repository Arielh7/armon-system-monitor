#include "colors.h"

const char *color_for_percent(float percent) {
    if (percent < 50.0f) return COLOR_GREEN;
    if (percent < 80.0f) return COLOR_YELLOW;
    return COLOR_RED;
}