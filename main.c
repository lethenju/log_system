/*
 * This file is an example to the log system behaviour. It shows how to initiates it, 
 * and briefly how to use it
 */ 
#include "log_system.h"

int main(int argc, char* argv[]) {
    log_init();
    log_add(DEBUG,"test %s","Debug test");
    usleep(450000);
    log_add(INFO,"test %s","Info test");
    usleep(100000);
    log_add(ERROR,"Error n %d, critical issue %d", 11, 45);
    usleep(230000);
    log_add(WARNING,"WARN%cNG",'I');
    
    
    pthread_join(log_pthread,NULL);

    return 0;
}
