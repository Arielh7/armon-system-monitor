#include <stdio.h>
#include "dashboard.h"
#include <unistd.h>

#include <stdio.h>
#include <unistd.h>
#include "dashboard.h"

int main(void) {
    // Enter alternate screen buffer so the dashboard doesn't pollute
    // the terminal scrollback history.
    printf("\033[?1049h");

    // Hide the cursor while the dashboard is running.
    printf("\033[?25l");

    while (1) {
        SystemInfo sys_info = system_information();

        // Move cursor to top-left without clearing the screen,
        // so each frame overwrites the previous one (no flicker).
        printf("\033[H");

        dashboard_run(&sys_info);

        // Refresh once per second.
        sleep(1);
    }

    return 0;
}


