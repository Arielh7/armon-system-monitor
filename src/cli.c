#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "cli.h"

#define ARMON_VERSION "0.3"

int cli_parse(int argc, char *argv[], CliOptions *opts) {
    opts->show_help    = false;
    opts->show_version = false;
    opts->one_shot     = false;
    opts->interval     = 1;

    for (int i = 1; i < argc; i++) {
        const char *arg = argv[i];

        if (strcmp(arg, "--help") == 0 || strcmp(arg, "-h") == 0) {
            opts->show_help = true;
        }
        else if (strcmp(arg, "--version") == 0 || strcmp(arg, "-v") == 0) {
            opts->show_version = true;
        }
        else if (strcmp(arg, "--oneshot") == 0 || strcmp(arg, "-o") == 0) {
            opts->one_shot = true;
        }
        else if (strcmp(arg, "--interval") == 0 || strcmp(arg, "-i") == 0) {
            if (i + 1 >= argc) {
                fprintf(stderr, "Error: --interval requires a value\n");
                return -1;
            }
            int n = atoi(argv[++i]);
            if (n <= 0) {
                fprintf(stderr, "Error: --interval must be a positive integer\n");
                return -1;
            }
            opts->interval = n;
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
    printf("  -h, --help           Show this help message and exit\n");
    printf("  -v, --version        Show version and exit\n");
    printf("  -o, --oneshot        Print the dashboard once and exit\n");
    printf("  -i, --interval N     Refresh every N seconds (default: 1)\n");
    printf("\n");
    printf("Examples:\n");
    printf("  armon                Live dashboard, refresh every second\n");
    printf("  armon -i 5           Live dashboard, refresh every 5 seconds\n");
    printf("  armon -o             Print once and exit\n");
}

void cli_print_version(void) {
    printf("ARMON - System Monitor v%s\n", ARMON_VERSION);
}