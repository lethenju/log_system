#ifndef LOG_SYSTEM
#define LOG_SYSTEM
#include <time.h>
#include <stdio.h>
#include <pthread.h>

#define __FILENAME__ (strrchr(__FILE__, '/') ? strrchr(__FILE__, '/') + 1 : __FILE__)
#define NUMARGS(...)  (sizeof((int[]){__VA_ARGS__})/sizeof(int))
#define MAX_SIZE_STACK 2048
pthread_t log_pthread;

enum log_level
{
    ERROR = 0,
    WARNING = 1,
    INFO = 2,
    DEBUG = 3
};

/** Adds a log in the system
 */ 
int log_add(int number_args, int level, const char* file, const char* func, const int line, const char* message, ...);

#define LOG_DEBUG(...) \
    do { log_add(NUMARGS(__VA_ARGS__),DEBUG, __FILE__, __func__, __LINE__, __VA_ARGS__); } while(0)
#define LOG_INFO(...) \
    do { log_add(NUMARGS(__VA_ARGS__), INFO, __VA_ARGS__); } while(0)
#define LOG_WARNING(...) \
    do { log_add(NUMARGS(__VA_ARGS__), WARNING, __VA_ARGS__); } while(0)
#define LOG_ERROR(...) \
    do { log_add(NUMARGS(__VA_ARGS__), ERROR, __VA_ARGS__); } while(0)

struct log
{
    int level;
    time_t time;
    char *func;
    char *file;
    int line;
    char *data;
};

/** Initialize the log system
 */
void log_init();

/** Main loop of the log system.
 */
void *log_thread(void);
/** Handles a log by writing it on output
 */
void log_handle(struct log *l, FILE *output);

#endif
