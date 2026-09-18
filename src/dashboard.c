#include <stdio.h>
#include "dashboard.h"
#include <string.h>
#include "render.h"
#include <sys/statvfs.h>
#include <unistd.h>   
#include <sys/utsname.h>  

static unsigned long prev_idle  = 0;
static unsigned long prev_total = 0;

static void read_system_info(SystemInfo *sys_info) {
    
    if (gethostname(sys_info->hostname, sizeof(sys_info->hostname)) != 0) {
        snprintf(sys_info->hostname, sizeof(sys_info->hostname), "unknown");
    }

    struct utsname uts;
    if (uname(&uts) == 0) {
        snprintf(sys_info->kernel, sizeof(sys_info->kernel),
                 "%s %s", uts.sysname, uts.release);
    } else {
        snprintf(sys_info->kernel, sizeof(sys_info->kernel), "unknown");
    }
}


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
    sys_info->swap_total_kb = 0; 
    sys_info->swap_used_kb  = 0;

    FILE *fp = fopen("/proc/meminfo", "r");
    if (!fp) {
        return;
    }

    unsigned long total = 0;
    unsigned long available = 0;
    unsigned long swap_total = 0;  
    unsigned long swap_free  = 0; 

    char line[256];
    while (fgets(line, sizeof(line), fp)) {
        if (strncmp(line, "MemTotal:", 9) == 0) {
            sscanf(line, "MemTotal: %lu", &total);
        }
        else if (strncmp(line, "MemAvailable:", 13) == 0) {
            sscanf(line, "MemAvailable: %lu", &available);
        }

         else if (strncmp(line, "SwapTotal:", 10) == 0) {   
            sscanf(line, "SwapTotal: %lu", &swap_total);
        }
        else if (strncmp(line, "SwapFree:", 9) == 0) {  
            sscanf(line, "SwapFree: %lu", &swap_free);
        }
    }

    fclose(fp);

    if (total == 0) {
        return;
    }

    sys_info->ram_total_kb = total;
    sys_info->ram_used_kb  = total - available;
    sys_info->swap_total_kb = swap_total;              
    sys_info->swap_used_kb  = swap_total - swap_free; 
}

static void read_loadavg(SystemInfo *sys_info) {
    sys_info->load_1min  = 0.0f;
    sys_info->load_5min  = 0.0f;
    sys_info->load_15min = 0.0f;

    FILE *fp = fopen("/proc/loadavg", "r");
    if (!fp) {
        return;
    }

    fscanf(fp, "%f %f %f",
           &sys_info->load_1min,
           &sys_info->load_5min,
           &sys_info->load_15min);
    fclose(fp);
}

static void read_cpu_cores(SystemInfo *sys_info) {
    long cores = sysconf(_SC_NPROCESSORS_ONLN);
    sys_info->cpu_cores = (cores > 0) ? (int)cores : 1;
}

SystemInfo system_information() {
    SystemInfo sys_info;
    
    sys_info.cpu_usage = read_cpu_usage();
    sys_info.uptime = read_uptime();
    read_ram_info(&sys_info);
    read_disk_info(&sys_info);
    read_system_info(&sys_info);
    read_loadavg(&sys_info);
    read_cpu_cores(&sys_info);

    return sys_info;
}
