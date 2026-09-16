#ifndef DASHBOARD_H
#define DASHBOARD_H


typedef struct {
    float cpu_usage;
    float ram_usage;
    float disk_usage;
    unsigned long uptime;
} SystemInfo;

void dashboard_run(const SystemInfo *sys_info);
SystemInfo system_information();

#endif