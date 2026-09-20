#ifndef PROCESSES_H
#define PROCESSES_H

#include "dashboard.h"


int processes_read(TopProcess *procs, int max);


void processes_get_top_ram(SystemInfo *sys_info, TopProcess *procs, int count, int n);

#endif