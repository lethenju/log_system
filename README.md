# log_system

 When designing applications, one of the most useful debugging tool is to trace
what is happening in the application.
For that, you could simply use a printf() function, but you will soon feel
bloated in loads of messages.

One idea is to create a leveled log system, with error logs, warning logs etc,
and decide to show only what you need.

But no need of a real project for that, a simple solution is to use macros..

## So whats the problem ?

We have all been through the famous situation where a million logs comes in a second
we need to pause everything and read it through, its going too fast !

Also, imagine you're working on a TUI project. The applications renders in the terminal 
directly. You can't use your stdout for logging, of course..

## What can I do ?

Thats where log_system comes handy :

The log_system is buffered. When you log many messages at once, you can specify 
a maximum speed to never feel totally bloated with no time to read logs!

You can also write logs to a open socket, 
and if a log_system server application is open elsewhere, your logs will come here !

You can also choose to write in a file !

## What is log_system actually ?

Log_system is a simple module for managing a logging system to log
your messages on different outputs (files, stdout..)

It is also a server, to show your log messages in real time in another app !
(or even remotely if you open your ports ! )

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
write_on_socket=1 ; write on a socket instead of stdout
output_file=test ; if write_on_file=1, name of the output file
level=4 ; add minimal log level 
smooth_end=1 ; wait for the end of the logs to end the process
```
