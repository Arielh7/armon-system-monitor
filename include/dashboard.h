#ifndef DASHBOARD_H
#define DASHBOARD_H

typedef struct {
    float cpu_usage;
    unsigned long ram_total_kb;
    unsigned long ram_used_kb; 
     unsigned long swap_total_kb; 
    unsigned long swap_used_kb;  
    unsigned long disk_total_kb;
    unsigned long disk_used_kb;
    unsigned long uptime;
    char hostname[64]; 
    char kernel[256]; 
    float load_1min;      
    float load_5min;      
    float load_15min;     
    int   cpu_cores;   
    
} SystemInfo;

SystemInfo system_information();

#endif