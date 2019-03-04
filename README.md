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

    DEBUG_TRACE("test %s","Debug test");
    usleep(450000);
    INFO_TRACE("test %s","Info test");
    usleep(100000);
    ERROR_TRACE("Error n %d, critical issue %d", 11, 45);
    usleep(230000);
    WARNING_TRACE("WARNING!!");


    log_end();
    return 0;
}
```
## Config file

Thanks to the module [inih](https://github.com/benhoyt/inih) , there is a configuration file management to control several parameters.

Here is a example of config file used in the project 

```ini
; config file for log_system

[config]
stack_size=4096 ; maximum logs in the stack
write_on_file=0 ; write on a file instead of stdout
output_file=logs  ; if write_on_file=1, name of the output file 
```


## Upcoming features 

- Add compile-time options
