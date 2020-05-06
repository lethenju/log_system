/*
 * This file is an example to the log system behaviour. It shows how to initiates it, 
 * and briefly how to use it
 */ 
#include <unistd.h>
#include "log_system.h"

pthread_t test, test2, test3;
void* thread_test(void) {
    int i;
    for (i=0; i<10; i++) {
        nanosleep((const struct timespec[]){{0, (long)(30000)}}, NULL);
        DEBUG_TRACE("Log number : %d", i);
    }
}
void* thread_test2(void) {
    int i;
    for (i=0; i<10; i++) {
        nanosleep((const struct timespec[]){{0, (long)(20000)}}, NULL);
        ERROR_TRACE("Omg ! %d", i);
    }
}
void* thread_test3(void) {
    int i;
    for (i=0; i<10; i++) {
        nanosleep((const struct timespec[]){{0, (long)(10000)}}, NULL);
        INFO_TRACE("Info !!! %d ! ",i);
    }
}

int main(int argc, char* argv[]) {
    log_init();

    pthread_create(&test, NULL, (void*) thread_test, (void*) NULL);
    pthread_create(&test2, NULL, (void*) thread_test2, (void*) NULL);
    pthread_create(&test3, NULL, (void*) thread_test3, (void*) NULL);

    pthread_join(test, NULL);
    pthread_join(test2, NULL);
    pthread_join(test3, NULL);
    
    log_end();
    return 0;
}
