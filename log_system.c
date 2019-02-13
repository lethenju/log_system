#include "log_system.h"
#include "stdio.h"
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>

struct log_ctx *context;

void log_config_load()
{
    FILE *fd;
    fd= fopen(CONFIG_FILE,"r");
   	int c;
    int key_value=0;
}


/** Initialize the log system
 */
void log_init()
{
    context = (struct log_ctx *)malloc(sizeof(struct log_ctx));
    context->begin = clock();
    context->stack_log = (struct log *)malloc(MAX_SIZE_STACK * sizeof(struct log));
    context->nb_logs_in_stack = 0;
    context->end = 0;
    pthread_create(&log_pthread, NULL, (void*) log_thread, (void*) NULL);
}
/** Main loop of the log system.
 */
void *log_thread(void)
{
    int i;
    float pace;
    float wait;
    int _continue = 1;
    while (_continue)
    {
        if (context->nb_logs_in_stack > 0){
            pace = (context->nb_logs_in_stack*100) / (float)MAX_SIZE_STACK;
            wait =  (pace < 50)* TIME_BETWEEN_LOGS +
                   (pace >= 50)* (-((float)1/(float)2)*((pace - 50)*(pace - 50))+TIME_BETWEEN_LOGS);
            if (wait <0) wait =0;
            usleep((int)(wait*1000));
            //printf("pace %f, wait %f", pace, wait); 
            log_handle(context->stack_log, stdout);
            context->nb_logs_in_stack--;
            for (i = 1; i < context->nb_logs_in_stack+1; i++)
            {
                *(context->stack_log + i - 1 ) = *(context->stack_log + i); 
            }
        } else if (context->end) {
            _continue = 0;
        }
    }
}
/** Handles a log by writing it on output
 */
int log_add(int level, char* format, ...)
{
    va_list valist;

    /* initialize valist for num number of arguments */
    va_start(valist, format);
    char* msg = (char*) malloc(sizeof(valist)+sizeof(format));
    vsprintf(msg, format,valist);
    va_end(valist);
    struct log l = {
       level,
       (double)(clock()-context->begin),
       msg
    };
    if (context->nb_logs_in_stack >= MAX_SIZE_STACK)
        return -1;
    *(context->stack_log + context->nb_logs_in_stack) = l;
    context->nb_logs_in_stack++;

   
    return 0;
}
/** Handles a log by writing it on output
 */
void log_handle(struct log *l, struct _IO_FILE *output)
{
    switch (l->level)
    {
    case ERROR: 
        fprintf(output, "\e[31m[%.2f] %s\e[39m\n", (double)l->time, l->data);
        break;
    case WARNING:
        fprintf(output, "\e[33m[%.2f] %s\e[39m\n", (double)l->time, l->data);
        break;
    case INFO:
        fprintf(output, "\e[34m[%.2f] %s\e[39m\n", (double)l->time, l->data);
        break;
    case DEBUG:
        fprintf(output, "\e[32m[%.2f] %s\e[39m\n", (double)l->time, l->data);
        break;
    default:
        break;
    }
}

void log_end() {
    context->end = 1;
    pthread_join(log_pthread,NULL);
    free(context->stack_log);
    free(context);

}
