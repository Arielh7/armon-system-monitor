#include <stdio.h>
#include <unistd.h>
#include <signal.h>
#include <stdlib.h>
#include "dashboard.h"
#include "render.h"
#include "splash.h"

static void cleanup(int sig) {
    (void)sig;

    printf("\033[?25h");
    printf("\033[?1049l"); 
    exit(0);
}

int main(void) {
    signal(SIGINT, cleanup);

    printf("\033[?1049h");
    printf("\033[?25l");


    printf("\033[2J\033[H");
    splash_screen();

  
    printf("\033[2J\033[H"); 

    while (1) {
        SystemInfo sys_info = system_information();
        printf("\033[H");
        dashboard_run(&sys_info);
        sleep(1);
    }

    return 0;
}