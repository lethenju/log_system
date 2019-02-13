#include <unistd.h>
#include "log_system.h"
pthread_t test, test2, test3;
void* thread_test(void) {
    int i;
    for (i=0; i<100; i++) {
        usleep(30);
        log_add(WARNING,"Log number : %d", i);
    }
    for (i=0; i<1000; i++) {
        usleep(30);
        log_add(WARNING,"Log number : %d", i);
    }
}
void* thread_test2(void) {
    int i;
    for (i=0; i<200; i++) {
        usleep(20);
        log_add(ERROR,"Log number : %d", i);
    }
}
void* thread_test3(void) {
    int i;
    for (i=0; i<100; i++) {
        usleep(10);
        log_add(INFO,"Log number : %d", i);
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
