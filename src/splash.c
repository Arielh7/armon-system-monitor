#include <stdio.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <string.h>
#include "splash.h"
#include "colors.h"

static void print_spaces(int n) {
    for (int i = 0; i < n; i++) putchar(' ');
}

static void print_centered(const char *text, int cols) {
    int len = strlen(text);
    int pad = (cols - len) / 2;
    if (pad < 0) pad = 0;
    print_spaces(pad);
    printf("%s%s\n", COLOR_CYAN, text);
}

void splash_screen(void) {
    struct winsize w;
    ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);
    int rows = w.ws_row;
    int cols = w.ws_col;

    int splash_height = 12;
    int top_pad = (rows - splash_height) / 2;
    if (top_pad < 0) top_pad = 0;

    for (int i = 0; i < top_pad; i++) printf("\n");
print_centered("  ___  ____  __  __  ___  _   _ ", cols);
print_centered(" / _ \\|  _ \\|  \\/  |/ _ \\| \\ | |", cols);
print_centered("| |_| | |_) | |\\/| | | | |  \\| |", cols);
print_centered("|  _  |  _ <| |  | | |_| | |\\  |", cols);
print_centered("|_| |_|_| \\_\\_|  |_|\\___/|_| \\_|", cols);

    printf("\n");
    print_centered("system-monitor", cols);
    print_centered("v0.2", cols);
    printf("\n\n");
    
    const char *init_text = "Initializing";
    int init_len = strlen(init_text);
    int init_pad = (cols - init_len - 3) / 2;
    if (init_pad < 0) init_pad = 0;

    print_spaces(init_pad);
    printf("%s", init_text);
    fflush(stdout);

    for (int i = 0; i < 3; i++) {
    usleep(500000);   
    printf(".");
    fflush(stdout);
}
printf("%s\n", COLOR_RESET);
usleep(500000);       
}

