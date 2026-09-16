#include <stdio.h>
#include "dashboard.h"
#include <string.h>

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

static void read_ram_info(SystemInfo *sys_info) {
    sys_info->ram_total_kb = 0;
    sys_info->ram_used_kb  = 0;

    FILE *fp = fopen("/proc/meminfo", "r");
    if (!fp) {
        return;
    }

    unsigned long total = 0;
    unsigned long available = 0;

    char line[256];
    while (fgets(line, sizeof(line), fp)) {
        if (strncmp(line, "MemTotal:", 9) == 0) {
            sscanf(line, "MemTotal: %lu", &total);
        }
        else if (strncmp(line, "MemAvailable:", 13) == 0) {
            sscanf(line, "MemAvailable: %lu", &available);
        }
        if (total > 0 && available > 0) {
            break;
        }
    }

    fclose(fp);

    if (total == 0) {
        return;
    }

    sys_info->ram_total_kb = total;
    sys_info->ram_used_kb  = total - available;
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

    if (sys_info->ram_total_kb > 0) {
        float used_gb  = (float)sys_info->ram_used_kb  / (1024.0f * 1024.0f);
        float total_gb = (float)sys_info->ram_total_kb / (1024.0f * 1024.0f);
        float percent  = (float)sys_info->ram_used_kb
                       / (float)sys_info->ram_total_kb * 100.0f;
        printf("RAM:    %.2f GB / %.2f GB (%.1f%%)\n",
               used_gb, total_gb, percent);
    } else {
        printf("RAM:    N/A\n");
    }

    printf("Disk:   %.2f%%\n", sys_info->disk_usage);
    printf("Uptime: ");
    print_uptime(sys_info->uptime);
    printf("\n");
}

SystemInfo system_information() {
    SystemInfo sys_info;
    
    sys_info.cpu_usage = 45.5;
    sys_info.disk_usage = 80.1;
    sys_info.uptime = read_uptime();
    read_ram_info(&sys_info);

    return sys_info;
}
