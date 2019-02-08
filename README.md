# log_system

Log_system is a simple module for managing a logging system to log
your messages on different outputs (files, stdout..)

It can buffer your logs to never feel bloated with too many messages.

## Usage

Here is a quick example of the usage of the trace system.

```c
#include <unistd.h>
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


    log_end();
    return 0;
}
```

## Upcoming features

- Config file to manage the stack size and the minimum timing between 2 logs, and how it should react to stack overflow.
- Output not only on stdout, but also in files

