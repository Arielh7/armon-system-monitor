#ifndef HEALTH_H
#define HEALTH_H

#include "dashboard.h"

typedef enum {HEALTH_OK, HEALTH_WARN, HEALTH_CRITICAL} HealthStatus;


HealthStatus health_compute(const SystemInfo *sys_info);

const char *health_label(HealthStatus status);

#endif