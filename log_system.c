#include "log_system.h"
#include "stdio.h"
#include <stdlib.h>
#include <stdarg.h>

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
        if (nb_logs_in_stack > 0){
            log_handle(stack_log, stdout);
            nb_logs_in_stack--;
            for (i = 1; i < nb_logs_in_stack+1; i++)
            {
                *(stack_log + i - 1 ) = *(stack_log + i); 
            }
        }
    }
}
/** Handles a log by writing it on output
 */
int log_add(int number_args, int level, const char* file, const char* func, const int line, const char* message, ...)
{
   va_list valist;

   /* initialize valist for num number of arguments */
   va_start(valist, number_args);
    switch (number_args) {
        case 2:
            sprintf(message, message, va_arg(valist, int)); 
    }
    struct log l = {
    level,
       time(NULL),
       func,
       file,
       line,
       message
    };
    if (nb_logs_in_stack >= MAX_SIZE_STACK)
        return -1;
    *(stack_log + nb_logs_in_stack) = l;
    nb_logs_in_stack++;
    return 0;
}
/** Handles a log by writing it on output
 */
void log_handle(struct log *l, struct _IO_FILE *output)
{
    switch (l->level)
    {
    case ERROR: // TODO Add color
        fprintf(output, "[%f][%s][%s][%d] %s\n", (double)time(&l->time), l->file, l->func, l->line, l->data);
        break;
    case WARNING:
        fprintf(output, "[%f][%s][%s][%d] %s\n", (double)time(&l->time), l->file, l->func, l->line, l->data);
        break;
    case INFO:
        fprintf(output, "[%f][%s][%s][%d] %s\n", (double)time(&l->time), l->file, l->func, l->line, l->data);
        break;
    case DEBUG:
        fprintf(output, "[%f][%s][%s][%d] %s\n", (double)time(&l->time), l->file, l->func, l->line, l->data);
        break;
    default:
        break;
    }
}
