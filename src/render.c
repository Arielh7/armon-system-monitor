#include <stdio.h>
#include <string.h>
#include <sys/ioctl.h>
#include <unistd.h>
#include "render.h"
#include "colors.h"
#include "health.h"

static void print_uptime(unsigned long uptime) {
    unsigned long hours   = uptime / 3600;
    unsigned long minutes = (uptime % 3600) / 60;
    unsigned long seconds = uptime % 60;

    printf("%luh %02lum %02lus", hours, minutes, seconds);
}

static void print_separator(int width) {
    printf(COLOR_CHROME);
    for (int i = 0; i < width; i++) {
        printf("\xE2\x94\x81");
    }
    printf(COLOR_RESET "\033[K\n");
}

static void print_section_title(const char *title, int width) {
    printf(COLOR_BOLD COLOR_CHROME " \xE2\x96\xB8 %s" COLOR_RESET "\033[K\n", title);
    print_separator(width);
}

void dashboard_run(const SystemInfo *sys_info) {
    printf("\n" COLOR_BOLD COLOR_CHROME "ARMON - System Monitor v0.3" COLOR_RESET "\033[K\n\n");

    char header_line[512];
    snprintf(header_line, sizeof(header_line),
             "Host: %s  |  Kernel: %s",
             sys_info->hostname, sys_info->kernel);

    printf(COLOR_LABEL "%s" COLOR_RESET "\033[K\n", header_line);
    int sep_width = strlen(header_line);
    print_separator(sep_width);
    printf("\033[K\n");


    HealthStatus health = health_compute(sys_info);
    const char *health_color;
    switch (health) {
        case HEALTH_OK:       health_color = COLOR_GREEN;  break;
        case HEALTH_WARN:     health_color = COLOR_YELLOW; break;
        case HEALTH_CRITICAL: health_color = COLOR_RED;    break;
    }
    printf(COLOR_LABEL "System Health:" COLOR_RESET "  %s\xE2\x97\x8F " COLOR_BOLD "%s" COLOR_RESET "\033[K\n\n",
           health_color,
           health_label(health));


    print_section_title("SYSTEM METRICS", sep_width);

    printf(COLOR_LABEL "%-15s" COLOR_RESET " %s" COLOR_BOLD "%.2f%%" COLOR_RESET "\033[K\n",
           "CPU Usage:",
           color_for_percent(sys_info->cpu_usage),
           sys_info->cpu_usage);

    if (sys_info->ram_total_kb > 0) {
        float used_gb  = (float)sys_info->ram_used_kb  / (1024.0f * 1024.0f);
        float total_gb = (float)sys_info->ram_total_kb / (1024.0f * 1024.0f);
        float percent  = (float)sys_info->ram_used_kb
                       / (float)sys_info->ram_total_kb * 100.0f;
        printf(COLOR_LABEL "%-15s" COLOR_RESET " " COLOR_BOLD "%.2f GB / %.2f GB" COLOR_RESET " (%s" COLOR_BOLD "%.1f%%" COLOR_RESET ")\033[K\n",
               "RAM Usage:", used_gb, total_gb,
               color_for_percent(percent), percent);
    } else {
        printf(COLOR_LABEL "%-15s" COLOR_RESET " N/A\033[K\n", "RAM Usage:");
    }

    if (sys_info->swap_total_kb > 0) {
        float used_gb  = (float)sys_info->swap_used_kb  / (1024.0f * 1024.0f);
        float total_gb = (float)sys_info->swap_total_kb / (1024.0f * 1024.0f);
        float percent  = (float)sys_info->swap_used_kb
                       / (float)sys_info->swap_total_kb * 100.0f;
        printf(COLOR_LABEL "%-15s" COLOR_RESET " " COLOR_BOLD "%.2f GB / %.2f GB" COLOR_RESET " (%s" COLOR_BOLD "%.1f%%" COLOR_RESET ")\033[K\n",
               "Swap Usage:", used_gb, total_gb,
               color_for_percent(percent), percent);
    } else {
        printf(COLOR_LABEL "%-15s" COLOR_RESET " N/A\033[K\n", "Swap Usage:");
    }

    if (sys_info->disk_total_kb > 0) {
        float used_gb  = (float)sys_info->disk_used_kb  / (1024.0f * 1024.0f);
        float total_gb = (float)sys_info->disk_total_kb / (1024.0f * 1024.0f);
        float percent  = (float)sys_info->disk_used_kb
                       / (float)sys_info->disk_total_kb * 100.0f;
        printf(COLOR_LABEL "%-15s" COLOR_RESET " " COLOR_BOLD "%.2f GB / %.2f GB" COLOR_RESET " (%s" COLOR_BOLD "%.1f%%" COLOR_RESET ")\033[K\n",
               "Disk Usage:", used_gb, total_gb,
               color_for_percent(percent), percent);
    } else {
        printf(COLOR_LABEL "%-15s" COLOR_RESET " N/A\033[K\n", "Disk Usage:");
    }

    printf(COLOR_LABEL "%-15s" COLOR_RESET " " COLOR_BOLD, "System Uptime:");
    print_uptime(sys_info->uptime);
    printf(COLOR_RESET "\033[K\n\n");


    print_section_title("LOAD AVERAGE", sep_width);

    printf(COLOR_LABEL "%-15s" COLOR_RESET " " COLOR_BOLD "%.2f  %.2f  %.2f" COLOR_RESET "   (1m / 5m / 15m)\033[K\n",
           "Load Average:",
           sys_info->load_1min,
           sys_info->load_5min,
           sys_info->load_15min);

    float load = sys_info->load_1min;
    int cores = sys_info->cpu_cores;

    const char *status;
    const char *status_color;
    if (load > cores * 2.0f) {
        status = "HIGH";
        status_color = COLOR_RED;
    } else if (load > cores) {
        status = "BUSY";
        status_color = COLOR_YELLOW;
    } else {
        status = "OK";
        status_color = COLOR_GREEN;
    }

    printf(COLOR_LABEL "%-15s" COLOR_RESET " %s" COLOR_BOLD "%s" COLOR_RESET " (%d cores)\033[K\n",
           "Load Status:", status_color, status, cores);


    printf("\033[K\n");
    print_section_title("TOP PROCESSES (by RAM)", sep_width);


    printf(COLOR_LABEL "   %-20s %6s %11s" COLOR_RESET "\033[K\n",
           "PROCESS", "PID", "MEMORY");

    for (int i = 0; i < sys_info->top_ram_count; i++) {
        float ram_mb = (float)sys_info->top_ram[i].ram_kb / 1024.0f;
        printf(COLOR_LABEL "%d. %-20s %6d %8.1f MB" COLOR_RESET "\033[K\n",
               i + 1,
               sys_info->top_ram[i].name,
               sys_info->top_ram[i].pid,
               ram_mb);
    }


    printf("\033[K\n");



}



void dashboard_run_short(const SystemInfo *sys_info) {

    printf("CPU: %.1f%%", sys_info->cpu_usage);

    if (sys_info->ram_total_kb > 0) {
        float percent = (float)sys_info->ram_used_kb
                      / (float)sys_info->ram_total_kb * 100.0f;
        printf("  RAM: %.1f%%", percent);
    } else {
        printf("  RAM: N/A");
    }

    if (sys_info->disk_total_kb > 0) {
        float percent = (float)sys_info->disk_used_kb
                      / (float)sys_info->disk_total_kb * 100.0f;
        printf("  Disk: %.1f%%", percent);
    } else {
        printf("  Disk: N/A");
    }

    unsigned long hours   = sys_info->uptime / 3600;
    unsigned long minutes = (sys_info->uptime % 3600) / 60;
    printf("  Uptime: %luh %02lum", hours, minutes);

    printf("\n");
}