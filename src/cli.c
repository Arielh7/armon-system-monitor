#include <stdio.h>
#include <string.h>
#include "cli.h"

#define ARMON_VERSION "0.3"

int cli_parse(int argc, char *argv[], CliOptions *opts) {
    opts->show_help    = false;
    opts->show_version = false;

    for (int i = 1; i < argc; i++) {
        const char *arg = argv[i];

        if (strcmp(arg, "--help") == 0 || strcmp(arg, "-h") == 0) {
            opts->show_help = true;
        }
        else if (strcmp(arg, "--version") == 0 || strcmp(arg, "-v") == 0) {
            opts->show_version = true;
        }
        else {
            fprintf(stderr, "Error: unknown option '%s'\n", arg);
            fprintf(stderr, "Try 'armon --help' for more information.\n");
            return -1;
        }
    }
    return 0;
}

void cli_print_help(void) {
    printf("ARMON - System Monitor v%s\n\n", ARMON_VERSION);
    printf("Usage: armon [options]\n\n");
    printf("Options:\n");
    printf("  -h, --help      Show this help message and exit\n");
    printf("  -v, --version   Show version and exit\n");
}

void cli_print_version(void) {
    printf("ARMON - System Monitor v%s\n", ARMON_VERSION);
}