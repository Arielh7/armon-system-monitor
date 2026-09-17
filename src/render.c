#include <stdio.h>
#include "render.h"
#include "colors.h"
#include "health.h"
#include <sys/ioctl.h>
#include <unistd.h>
#include <string.h>
#include <unistd.h>

static void print_uptime(unsigned long uptime) {
    unsigned long hours   = uptime / 3600;
    unsigned long minutes = (uptime % 3600) / 60;
    unsigned long seconds = uptime % 60;

    printf("%luh %02lum %02lus", hours, minutes, seconds);
}


void dashboard_run(const SystemInfo *sys_info) {

    printf("\nARMON - System Monitor v0.2\n\n");

    char header_line[512];
    snprintf(header_line, sizeof(header_line),
             "Host: %s  |  Kernel: %s",
             sys_info->hostname, sys_info->kernel);

    printf("%s\n", header_line);


    int len = strlen(header_line);
    for (int i = 0; i < len; i++) {
        printf("\xE2\x94\x80");   // ─
    }
    printf("\n\n");
     
  
    HealthStatus health = health_compute(sys_info);
    const char *health_color;
    switch (health) {
        case HEALTH_OK:       health_color = COLOR_GREEN;  break;
        case HEALTH_WARN:     health_color = COLOR_YELLOW; break;
        case HEALTH_CRITICAL: health_color = COLOR_RED;    break;
    }
    printf("System Health:  %s\xE2\x97\x8F %s%s\n\n",
           health_color,   
           health_label(health),
           COLOR_RESET);


    printf("%-15s %s%.2f%%%s\n",
       "CPU Usage:",
       color_for_percent(sys_info->cpu_usage),
       sys_info->cpu_usage,
       COLOR_RESET);

    if (sys_info->ram_total_kb > 0) {
        float used_gb  = (float)sys_info->ram_used_kb  / (1024.0f * 1024.0f);
        float total_gb = (float)sys_info->ram_total_kb / (1024.0f * 1024.0f);
        float percent  = (float)sys_info->ram_used_kb
                       / (float)sys_info->ram_total_kb * 100.0f;
        printf("%-15s %.2f GB / %.2f GB (%s%.1f%%%s)\n",
               "RAM Usage:", used_gb, total_gb, color_for_percent(percent), percent, COLOR_RESET);
    } else {
        printf("%-15s N/A\n", "RAM Usage:");
    }

    if (sys_info->swap_total_kb > 0) {
        float used_gb  = (float)sys_info->swap_used_kb  / (1024.0f * 1024.0f);
        float total_gb = (float)sys_info->swap_total_kb / (1024.0f * 1024.0f);
        float percent  = (float)sys_info->swap_used_kb
                       / (float)sys_info->swap_total_kb * 100.0f;
        printf("%-15s %.2f GB / %.2f GB (%s%.1f%%%s)\n",
               "Swap Usage:", used_gb, total_gb,
               color_for_percent(percent), percent, COLOR_RESET);
    } else {
        printf("%-15s N/A\n", "Swap Usage:");
    }

        if (sys_info->disk_total_kb > 0) {
        float used_gb  = (float)sys_info->disk_used_kb  / (1024.0f * 1024.0f);
        float total_gb = (float)sys_info->disk_total_kb / (1024.0f * 1024.0f);
        float percent  = (float)sys_info->disk_used_kb
                       / (float)sys_info->disk_total_kb * 100.0f;
        printf("%-15s %.2f GB / %.2f GB (%s%.1f%%%s)\n",
               "Disk Usage:", used_gb, total_gb,
               color_for_percent(percent), percent, COLOR_RESET);
    } else {
        printf("%-15s N/A\n", "Disk Usage:");
    }

    printf("%-15s ", "System Uptime:");
    print_uptime(sys_info->uptime);
    printf("\n");
}