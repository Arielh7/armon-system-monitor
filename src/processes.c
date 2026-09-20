#define _DEFAULT_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <dirent.h>
#include "processes.h"


static int read_one_process(int pid, TopProcess *out) {
    char path[64];

    snprintf(path, sizeof(path), "/proc/%d/cmdline", pid);
    FILE *fp = fopen(path, "r");
    if (!fp) return -1;

    char cmdline[4096];
    size_t n = fread(cmdline, 1, sizeof(cmdline) - 1, fp);
    fclose(fp);

    if (n == 0) return -1;
    cmdline[n] = '\0';


    size_t first_arg_len = 0;
    while (first_arg_len < n && cmdline[first_arg_len] != '\0') {
        first_arg_len++;
    }


    const char *last_slash = NULL;
    for (size_t i = 0; i < first_arg_len; i++) {
        if (cmdline[i] == '/') last_slash = &cmdline[i];
    }

    const char *basename = last_slash ? last_slash + 1 : cmdline;


    size_t name_len = strlen(basename);
    if (name_len >= sizeof(out->name)) {
        name_len = sizeof(out->name) - 1;
    }
    memcpy(out->name, basename, name_len);
    out->name[name_len] = '\0';

    if (out->name[0] == '\0') {
        snprintf(path, sizeof(path), "/proc/%d/comm", pid);
        fp = fopen(path, "r");
        if (!fp) return -1;
        if (!fgets(out->name, sizeof(out->name), fp)) {
            fclose(fp);
            return -1;
        }
        fclose(fp);

        size_t len = strlen(out->name);
        if (len > 0 && out->name[len - 1] == '\n') {
            out->name[len - 1] = '\0';
        }
    }


    snprintf(path, sizeof(path), "/proc/%d/status", pid);
    fp = fopen(path, "r");
    if (!fp) return -1;

    out->ram_kb = 0;
    char line[256];
    while (fgets(line, sizeof(line), fp)) {
        if (strncmp(line, "VmRSS:", 6) == 0) {
            sscanf(line, "VmRSS: %lu", &out->ram_kb);
            break;
        }
    }
    fclose(fp);

    out->pid = pid;
    return 0;
}

int processes_read(TopProcess *procs, int max) {
    DIR *dir = opendir("/proc");
    if (!dir) return 0;

    int count = 0;
    struct dirent *entry;

    while ((entry = readdir(dir)) != NULL && count < max) {
        if (!isdigit((unsigned char)entry->d_name[0])) continue;

        int pid = atoi(entry->d_name);

        TopProcess p;
        if (read_one_process(pid, &p) == 0) {
            procs[count++] = p;
        }
    }

    closedir(dir);
    return count;
}


static int cmp_by_ram(const void *a, const void *b) {
    const TopProcess *pa = a;
    const TopProcess *pb = b;
    if (pb->ram_kb > pa->ram_kb) return  1;
    if (pb->ram_kb < pa->ram_kb) return -1;
    return 0;
}

void processes_get_top_ram(SystemInfo *sys_info, TopProcess *procs, int count, int n) {
    qsort(procs, count, sizeof(TopProcess), cmp_by_ram);

    sys_info->top_ram_count = 0;
    for (int i = 0; i < n && i < count; i++) {
        sys_info->top_ram[i] = procs[i];
        sys_info->top_ram_count++;
    }
}