#include "log_system.h"
#include "stdio.h"
#include <stdlib.h>

struct log *stack_log;
int nb_logs_in_stack = 0;

/** Initialize the log system
 */
void log_init()
{
    stack_log = (struct log *)malloc(MAX_SIZE_STACK * sizeof(struct log));
    nb_logs_in_stack = 0;
}
/** Main loop of the log system.
 */
void *log_thread(void)
{
    int i;
    while (1)
    {
        log_handle(stack_log, stdout);
        for (i = 0; i < nb_logs_in_stack; i++)
        {
            *(stack_log + i) = *(stack_log + i + 1);
        }
    }
}
/** Handles a log by writing it on output
 */
int log_add(char* message)
{
    struct log l = {
        1,
       time(NULL),
       "test",
       "test", // TODO FIX THAT
       "test",
       message
    };
    if (nb_logs_in_stack >= MAX_SIZE_STACK)
        return -1;
    nb_logs_in_stack++;
    *(stack_log + nb_logs_in_stack) = l;
    return 0;
}
/** Handles a log by writing it on output
 */
void log_handle(struct log *l, struct _IO_FILE *output)
{
    switch (l->level)
    {
    ERROR:
        fprintf(output, "[%f][%s][%s][%s] %s", (double)time(&l->time), l->mod, l->file, l->func, l->data);
        break;
    WARNING:
        fprintf(output, "[%f][%s][%s][%s] %s", (double)time(&l->time), l->mod, l->file, l->func, l->data);
        break;
    INFO:
        fprintf(output, "[%f][%s][%s][%s] %s", (double)time(&l->time), l->mod, l->file, l->func, l->data);
        break;
    DEBUG:
        fprintf(output, "[%f][%s][%s][%s] %s", (double)time(&l->time), l->mod, l->file, l->func, l->data);
        break;
    default:
        break;
    }
}
