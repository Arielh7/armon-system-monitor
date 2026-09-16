#ifndef DASHBOARD_H
#define DASHBOARD_H


typedef struct {
    float cpu_usage;
    unsigned long ram_total_kb;
    unsigned long ram_used_kb; 
    float disk_usage;
    unsigned long uptime;
} SystemInfo;

void dashboard_run(const SystemInfo *sys_info);
SystemInfo system_information();

#endif