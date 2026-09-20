#include <stdio.h>
#include <unistd.h>
#include <signal.h>
#include <stdlib.h>
#include "dashboard.h"
#include "render.h"
#include "splash.h"
#include "cli.h"
#include "output.h"

static volatile sig_atomic_t resized = 0;

static void cleanup(int sig) {
    (void)sig;
    printf("\033[?25h");
    printf("\033[?1049l");
    exit(0);
}

static void on_resize(int sig) {
    (void)sig;
    resized = 1;
}

int main(int argc, char *argv[]) {
    CliOptions opts;
    if (cli_parse(argc, argv, &opts) != 0) {
        return 1;
    }

    if (opts.show_help)    { cli_print_help();    return 0; }
    if (opts.show_version) { cli_print_version(); return 0; }

    if (opts.one_shot) {
        SystemInfo sys_info = system_information();
        dashboard_run_short(&sys_info);
        return 0;
    }

    signal(SIGINT, cleanup);
    signal(SIGWINCH, on_resize);

    printf("\033[?1049h");
    printf("\033[?25l");

    printf("\033[2J\033[H");
    splash_screen();

    printf("\033[2J\033[H");

    while (1) {
        SystemInfo sys_info = system_information();

        out_begin();

        if (resized) {

            out_printf("\033[2J\033[H");
            resized = 0;
        } else {

            out_printf("\033[H");
        }

        dashboard_run(&sys_info);
        out_printf("\033[J");
        out_flush();

        sleep(opts.interval);
    }

    return 0;
}