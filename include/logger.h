#ifndef LOGGER_H
#define LOGGER_H

typedef enum {
    LOG_INFO,
    LOG_WARN,
    LOG_ERROR
} LogLevel;

void log_msg       (LogLevel level, const char *msg);
void log_process   (LogLevel level, int pid, const char *msg);
void log_scheduler (LogLevel level, const char *scheduler, int pid, int time);
void log_memory    (LogLevel level, int pid, int size, const char *action);

#endif