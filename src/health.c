#include "health.h"

static HealthStatus status_for_value(float percent, float warn, float critical) {
    if (percent >= critical) return HEALTH_CRITICAL;
    if (percent >= warn)     return HEALTH_WARN;
    return HEALTH_OK;
}

HealthStatus health_compute(const SystemInfo *sys_info) {
    HealthStatus worst = HEALTH_OK;

    HealthStatus cpu = status_for_value(sys_info->cpu_usage, 50.0f, 80.0f);
    if (cpu > worst) worst = cpu;

    if (sys_info->ram_total_kb > 0) {
        float ram_percent = (float)sys_info->ram_used_kb
                          / (float)sys_info->ram_total_kb * 100.0f;
        HealthStatus ram = status_for_value(ram_percent, 50.0f, 80.0f);
        if (ram > worst) worst = ram;
    }

    if (sys_info->disk_total_kb > 0) {
        float disk_percent = (float)sys_info->disk_used_kb
                           / (float)sys_info->disk_total_kb * 100.0f;
        HealthStatus disk = status_for_value(disk_percent, 80.0f, 90.0f);
        if (disk > worst) worst = disk;
    }

    return worst;
}

const char *health_label(HealthStatus status) {
    switch (status) {
        case HEALTH_OK:       return "GOOD";
        case HEALTH_WARN:     return "WARN";
        case HEALTH_CRITICAL: return "CRITICAL";
    }
    return "UNKNOWN";
}