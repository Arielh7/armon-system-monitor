#ifndef DASHBOARD_H
#define DASHBOARD_H

typedef struct {
    float cpu_usage;
    unsigned long ram_total_kb;
    unsigned long ram_used_kb; 
    unsigned long disk_total_kb;
    unsigned long disk_used_kb;
    unsigned long uptime;
} SystemInfo;

SystemInfo system_information();

#endif