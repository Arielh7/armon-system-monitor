#include <stdio.h>
#include "dashboard.h"
#include <unistd.h>
int main() {
 
    printf("\033[?1049h"); 
    printf("\033[?25l");    

    while (1) {
        SystemInfo sys_info = system_information();
        printf("\033[H");   
        dashboard_run(&sys_info);
        sleep(1);
    }

    return 0;
}


