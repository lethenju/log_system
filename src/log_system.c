#include "log_system.h"
#include "stdio.h"
#include "inih/ini.h"
#include <stdio.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include <unistd.h>
#define PORT 8080

struct log_ctx *context;

void log_handle(struct log *l, FILE *output);

void log_handle_file(struct log *l, FILE *output);

void log_handle_socket(struct log *l, int sock);

static int handler_ini(void *config, const char *section, const char *name,
                       const char *value)
{
    configuration *pconfig = (configuration *)config;

#define MATCH(s, n) strcmp(section, s) == 0 && strcmp(name, n) == 0
    if (MATCH("config", "stack_size"))
    {
        pconfig->stack_size = atoi(value);
    }
    else if (MATCH("config", "write_on_file"))
    {
        pconfig->write_on_file = atoi(value);
    }
    else if (MATCH("config", "write_on_socket"))
    {
        pconfig->write_on_socket = atoi(value);
    }
    else if (MATCH("config", "output_file"))
    {
        pconfig->output_file = strdup(value);
    }
    else if (MATCH("config", "smooth_end"))
    {
        pconfig->smooth_end = atoi(value);
    }
    else if (MATCH("config", "level"))
    {
        pconfig->level = atoi(value);
    }
    else
    {
        return 0; /* unknown section/name, error */
    }
    return 1;
}

int log_config_load(struct log_ctx *ctx)
{
    ctx->config = (configuration *)malloc(sizeof(configuration));
    if (ini_parse("config.ini", handler_ini, ctx->config) < 0)
    {
        printf("Can't load 'config.ini'\n");
        return -1;
    }
    printf("Config loaded from 'config.ini': stack_size=%d, write_on_file=%d,\
            write_on_socket=%d output_file=%s, level=%d\n",
           ctx->config->stack_size, ctx->config->write_on_file,
           ctx->config->write_on_socket, ctx->config->output_file, ctx->config->level);
    return 0;
}

/** Initialize the log system
 */
void log_init()
{
    context = (struct log_ctx *)malloc(sizeof(struct log_ctx));
    if (log_config_load(context) == -1)
    {
        // Default configuration
        context->config->stack_size = MAX_SIZE_STACK;
        context->config->write_on_file = 0;
        context->config->write_on_socket = 0;
        context->config->output_file = "";
    }
    else if (context->config->write_on_file)
    {
        context->fp = fopen(context->config->output_file, "w");
        fprintf(context->fp, "========\n");
        fclose(context->fp);
    }
    else if (context->config->write_on_socket)

    {
        // init co
        struct sockaddr_in address;
        context->socket = 0;
        int valread;
        struct sockaddr_in serv_addr;
        if ((context->socket = socket(AF_INET, SOCK_STREAM, 0)) < 0)
        {
            printf("\n Socket creation error \n");
        }

        memset(&serv_addr, '0', sizeof(serv_addr));

        serv_addr.sin_family = AF_INET;
        serv_addr.sin_port = htons(PORT);

        // Convert IPv4 and IPv6 addresses from text to binary form
        if (inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr) <= 0)
        {
            printf("\nInvalid address/ Address not supported \n");
        }

        if (connect(context->socket, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
        {
            printf("\nConnection Failed \n");
        }
        // Co inited
    }
    context->begin = clock();
    context->stack_log = (struct log *)malloc(context->config->stack_size * sizeof(struct log));
    context->nb_logs_in_stack = 0;
    context->end = 0;

    pthread_create(&log_pthread, NULL, (void *)log_thread, (void *)NULL);
}
/** Main loop of the log system.
 */
void *log_thread(void)
{

    int i;
    float pace;
    float wait;
    int _continue = 1;
    while (_continue)
    {
        if (context->nb_logs_in_stack > 0)
        {
            pace = (context->nb_logs_in_stack * 100) / (float)context->config->stack_size;
            wait = (pace < 50) * TIME_BETWEEN_LOGS +
                   (pace >= 50) * (-((float)1 / (float)2) * ((pace - 50) * (pace - 50)) + TIME_BETWEEN_LOGS);
            if (wait < 0)
                wait = 0;
            nanosleep((const struct timespec[]){{0, (long)(wait * 1000 * 1000)}}, NULL);

            if (context->stack_log->level <= context->config->level)
            {
                if (context->config->write_on_file)
                {
                    context->fp = fopen(context->config->output_file, "a");
                    log_handle_file(context->stack_log, context->fp);
                    fclose(context->fp);
                }
                else if (context->config->write_on_socket)
                {
                    log_handle_socket(context->stack_log, context->socket);
                }
                else
                {
                    log_handle(context->stack_log, stdout);
                }
            }
            context->nb_logs_in_stack--;
            for (i = 1; i < context->nb_logs_in_stack + 1; i++)
            {
                *(context->stack_log + i - 1) = *(context->stack_log + i);
            }
        }
        else if (context->end)
        {
            _continue = 0;
        }
        if (!context->config->smooth_end && context->end)
        {
            _continue = 0;
        }
    }
}
/** Handles a log by writing it on output
 */
int log_add(int level, int line, char *file, const char *function, char *format, ...)
{
    va_list valist;

    /* initialize valist for num number of arguments */
    va_start(valist, format);
    char *msg = malloc(sizeof(valist) + sizeof(format));
    vsprintf(msg, format, valist);
    va_end(valist);
    struct log l = {
        level,
        (double)(clock() - context->begin) / (float)CLOCKS_PER_SEC,
        line,
        file,
        function,
        msg};
    if (context->nb_logs_in_stack >= context->config->stack_size)
    {
        printf("\e[31m[LOG_SYSTEM] NO MORE ROOM IN LOG STACK !\e[39m\n");
        free(msg);
        return -1;
    }
    *(context->stack_log + context->nb_logs_in_stack) = l;
    context->nb_logs_in_stack++;
    return 0;
}
/** Handles a log by writing it on output
 */
void log_handle(struct log *l, struct _IO_FILE *output)
{
    switch (l->level)
    {
    case ERROR:
        fprintf(output, "\e[31m[%.2f] %s\e[39m\n", (double)l->time, l->data);
        break;
    case WARNING:
        fprintf(output, "\e[33m[%.2f] %s\e[39m\n", (double)l->time, l->data);
        break;
    case INFO:
        fprintf(output, "\e[34m[%.2f] %s\e[39m\n", (double)l->time, l->data);
        break;
    case DEBUG:
        fprintf(output, "\e[32m[%.2f] %s\e[39m\n", (double)l->time, l->data);
        break;
    default:
        break;
    }
}

/** Handles a log by writing it on a socket
 */
void log_handle_socket(struct log *l, int sock)
{
    char *output = malloc(1024);
    switch (l->level)
    {
    case ERROR:
        sprintf(output, "\e[31m[%.2f][%s:%d][%s] %s\e[39m\n", (double)l->time, l->file, l->line, l->func, l->data);
        break;
    case WARNING:
        sprintf(output, "\e[33m[%.2f][%s:%d][%s] %s\e[39m\n", (double)l->time, l->file, l->line, l->func, l->data);
        break;
    case INFO:
        sprintf(output, "\e[34m[%.2f][%s:%d][%s] %s\e[39m\n", (double)l->time, l->file, l->line, l->func, l->data);
        break;
    case DEBUG:
        sprintf(output, "\e[32m[%.2f][%s:%d][%s] %s\e[39m\n", (double)l->time, l->file, l->line, l->func, l->data);
        break;
    default:
        break;
    }
    send(sock, output, strlen(output), 0);
    free(output);
}

/** Handles a log by writing it on output
 */
void log_handle_file(struct log *l, struct _IO_FILE *output)
{
    switch (l->level)
    {
    case ERROR:
        fprintf(output, "[%.2f] %s\n", (double)l->time, l->data);
        break;
    case WARNING:
        fprintf(output, "[%.2f] %s\n", (double)l->time, l->data);
        break;
    case INFO:
        fprintf(output, "[%.2f] %s\n", (double)l->time, l->data);
        break;
    case DEBUG:
        fprintf(output, "[%.2f] %s\n", (double)l->time, l->data);
        break;
    default:
        break;
    }
}

/** Set a new log level
 */
void set_log_level(int level)
{
    context->config->level = level;
}

/** Get current log level
 */
int get_log_level()
{
    return context->config->level;
}

void log_end()
{
    context->end = 1;
    pthread_join(log_pthread, NULL);
    if (context->config->write_on_file)
        fclose(context->fp);
    free(context->stack_log);
    free(context);
}
