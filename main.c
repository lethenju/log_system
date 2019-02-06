/*
 * This file is an example to the log system behaviour. It shows how to initiates it, 
 * and briefly how to use it
 */ 
#include "log_system.h"

int main(int argc, char* argv[]) {
    log_init();
    pthread_create(&log_pthread, NULL, (void*) log_thread, (void*) NULL);
    

    LOG_DEBUG("test %d", 2);
    
    
    pthread_join(log_pthread,NULL);
    return 0;
}
