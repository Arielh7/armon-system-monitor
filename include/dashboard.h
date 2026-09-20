#ifndef DASHBOARD_H
#define DASHBOARD_H
#define TOP_PROCESSES_MAX 256
#define TOP_N 5

typedef struct {
    int pid;
    char name[64];
    unsigned long ram_kb;
} TopProcess;

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
        TopProcess top_ram[TOP_N]; 
    int top_ram_count;
} SystemInfo;

SystemInfo system_information();

#endif