#ifndef LOG_SYSTEM
#define LOG_SYSTEM
#include <time.h>
#include <stdio.h>
#include <pthread.h>
#define MAX_SIZE_FUNC 256
#define MAX_SIZE_FILE 256
#define MAX_SIZE_MOD 256
#define MAX_SIZE_DATA 1024
#define MAX_SIZE_STACK 2048

pthread_t log_pthread;

enum log_level
{
    ERROR = 0,
    WARNING = 1,
    INFO = 2,
    DEBUG = 3
};

struct log
{
    int level;
    time_t time;
    char *func;
    char *file;
    char *mod;
    char *data;
};

/** Initialize the log system
 */
void log_init();
/** Adds a log in the system
 */ 
int log_add(char* message);
/** Main loop of the log system.
 */
void *log_thread(void);
/** Handles a log by writing it on output
 */
void log_handle(struct log *l, FILE *output);

#endif