#include <stdio.h>
#include "dashboard.h"
#include <string.h>
#include "render.h"
#include <sys/statvfs.h>


static unsigned long prev_idle  = 0;
static unsigned long prev_total = 0;

static void read_disk_info(SystemInfo *sys_info) {
    sys_info->disk_total_kb = 0;
    sys_info->disk_used_kb  = 0;

    struct statvfs info;
    if (statvfs("/", &info) != 0) {
        return;
    }

    unsigned long block_size = info.f_frsize;
    unsigned long total_bytes = info.f_blocks * block_size;
    unsigned long avail_bytes = info.f_bavail * block_size;

    sys_info->disk_total_kb = total_bytes / 1024;
    sys_info->disk_used_kb  = (total_bytes - avail_bytes) / 1024;
}


static float read_cpu_usage(void) {
    FILE *fp = fopen("/proc/stat", "r");
    if (!fp) {
        return 0.0f;
    }

    char label[16];
    unsigned long user, nice, system, idle, iowait, irq, softirq, steal;
    int matched = fscanf(fp, "%15s %lu %lu %lu %lu %lu %lu %lu %lu", label, &user, &nice, &system, &idle, &iowait, &irq, &softirq, &steal);
    fclose(fp);

    if (matched < 9) {
        return 0.0f;
    }

    unsigned long idle_time = idle + iowait;
    unsigned long total = user + nice + system + idle + iowait + irq + softirq + steal;

    if (prev_total == 0) {
        prev_idle  = idle_time;
        prev_total = total;
        return 0.0f;
    }

    unsigned long d_idle  = idle_time - prev_idle;
    unsigned long d_total = total - prev_total;

    prev_idle  = idle_time;
    prev_total = total;

    if (d_total == 0) {
        return 0.0f;
    }

    float usage = 1.0f - ((float)d_idle / (float)d_total);
    return usage * 100.0f;
}




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





SystemInfo system_information() {
    SystemInfo sys_info;
    
    sys_info.cpu_usage = read_cpu_usage();
    sys_info.uptime = read_uptime();
    read_ram_info(&sys_info);
    read_disk_info(&sys_info);

    return sys_info;
}
