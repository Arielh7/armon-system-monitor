#include <stdio.h>
#include "dashboard.h"

static unsigned long read_uptime(void) {
    FILE *fp = fopen("/proc/uptime", "r");
    if (!fp) {
        return 0;
    }

    unsigned long seconds = 0;
    fscanf(fp, "%lu", &seconds);
    fclose(fp);

    return seconds;
}

static void print_uptime(unsigned long uptime) {
    unsigned long hours   = uptime / 3600;
    unsigned long minutes = (uptime % 3600) / 60;
    unsigned long seconds = uptime % 60;

    printf("%luh %02lum %02lus", hours, minutes, seconds);
}

void dashboard_run(const SystemInfo *sys_info) {
    printf("ARIEL SYSTEM MONITOR\n");
    printf("--------------------\n\n");
    printf("CPU:    %.2f%%\n", sys_info->cpu_usage);
    printf("RAM:    %.2f%%\n", sys_info->ram_usage);
    printf("Disk:   %.2f%%\n", sys_info->disk_usage);
    printf("Uptime: ");
    print_uptime(sys_info->uptime);
    printf("\n");
}

SystemInfo system_information() {
    SystemInfo sys_info;
    sys_info.cpu_usage = 45.5;
    sys_info.ram_usage = 65.2;
    sys_info.disk_usage = 80.1;
    sys_info.uptime = read_uptime();

    return sys_info;
}
