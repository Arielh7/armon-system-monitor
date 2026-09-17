#include <stdio.h>
#include "render.h"
#include "dashboard.h"

static void print_uptime(unsigned long uptime) {
    unsigned long hours   = uptime / 3600;
    unsigned long minutes = (uptime % 3600) / 60;
    unsigned long seconds = uptime % 60;

    printf("%luh %02lum %02lus", hours, minutes, seconds);
}
void dashboard_run(const SystemInfo *sys_info) {
    printf("ARMON - System Monitor v0.1\n");
    printf("===========================\n\n");

    printf("%-15s %.2f%%\n", "CPU Usage:", sys_info->cpu_usage);

    if (sys_info->ram_total_kb > 0) {
        float used_gb  = (float)sys_info->ram_used_kb  / (1024.0f * 1024.0f);
        float total_gb = (float)sys_info->ram_total_kb / (1024.0f * 1024.0f);
        float percent  = (float)sys_info->ram_used_kb
                       / (float)sys_info->ram_total_kb * 100.0f;
        printf("%-15s %.2f GB / %.2f GB (%.1f%%)\n",
               "RAM Usage:", used_gb, total_gb, percent);
    } else {
        printf("%-15s N/A\n", "RAM Usage:");
    }

    if (sys_info->disk_total_kb > 0) {
    float used_gb  = (float)sys_info->disk_used_kb  / (1024.0f * 1024.0f);
    float total_gb = (float)sys_info->disk_total_kb / (1024.0f * 1024.0f);
    float percent  = (float)sys_info->disk_used_kb
                   / (float)sys_info->disk_total_kb * 100.0f;
    printf("%-15s %.2f GB / %.2f GB (%.1f%%)\n",
           "Disk Usage:", used_gb, total_gb, percent);
} else {
    printf("%-15s N/A\n", "Disk Usage:");
}

    printf("%-15s ", "System Uptime:");
    print_uptime(sys_info->uptime);
    printf("\n");
}