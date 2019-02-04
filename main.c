/*
 * This file is an example to the log system behaviour. It shows how to initiates it
 * 
 */ 

#include "log_system.h"
#include "stdio.h"

int main(int argc, char* argv[]) {
    log_init();
    pthread_create(&log_pthread, NULL, (void*) log_thread, (void*) NULL);
    
    log_add("test");
    
    
    pthread_join(log_pthread,NULL);
    return 0;
}