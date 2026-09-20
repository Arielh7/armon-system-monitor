#ifndef CLI_H
#define CLI_H

#include <stdbool.h>

typedef struct {
    bool show_help;      
    bool show_version; 
    bool one_shot;     
    int  interval;        
} CliOptions;

int cli_parse(int argc, char *argv[], CliOptions *opts);

void cli_print_help(void);

void cli_print_version(void);

#endif