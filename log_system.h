#ifndef LOG_SYSTEM
#define LOG_SYSTEM
#include <time.h>
#include <stdio.h>
#include <pthread.h>

#define __FILENAME__ (strrchr(__FILE__, '/') ? strrchr(__FILE__, '/') + 1 : __FILE__)
#define MAX_SIZE_STACK 2048
pthread_t log_pthread;

enum log_level
{
    ERROR = 0,
    WARNING = 1,
    INFO = 2,
    DEBUG = 3
};

int log_add(int level, char* format,  ...);

struct log_ctx
{
    struct log *stack_log;
    int nb_logs_in_stack;
    clock_t begin;
};

struct log
{
    int level;
    double time;
    char *data;
};

/** Initialize the log system
 */
void log_init();
/** Adds a log in the system
 */ 
/** Main loop of the log system.
 */
void *log_thread(void);
/** Handles a log by writing it on output
 */
void log_handle(struct log *l, FILE *output);

void log_end();
#endif
