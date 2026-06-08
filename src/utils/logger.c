#include <stdio.h>
#include <time.h>
#include "logger.h"
 
static const char *level_str(LogLevel level) {
    switch (level) {
        case LOG_INFO:  return "INFO ";
        case LOG_WARN:  return "WARN ";
        case LOG_ERROR: return "ERROR";
        default:        return "INFO ";
    }
}
 
static void timestamp(char *buf, int size) {
    time_t now = time(NULL);
    struct tm *t = localtime(&now);
    strftime(buf, size, "%H:%M:%S", t);
}
 
void log_msg(LogLevel level, const char *msg) {
    if (!msg) return;
    char ts[16];
    timestamp(ts, sizeof(ts));
    printf("[%s] [%s] %s\n", ts, level_str(level), msg);
}
 
void log_process(LogLevel level, int pid, const char *msg) {
    if (!msg) return;
    char ts[16];
    timestamp(ts, sizeof(ts));
    printf("[%s] [%s] PID %d: %s\n", ts, level_str(level), pid, msg);
}
 
void log_scheduler(LogLevel level, const char *scheduler, int pid, int time) {
    char ts[16];
    timestamp(ts, sizeof(ts));
    printf("[%s] [%s] [%s] PID %d en tiempo %d\n", ts, level_str(level), scheduler, pid, time);
}
 
void log_memory(LogLevel level, int pid, int size, const char *action) {
    if (!action) return;
    char ts[16];
    timestamp(ts, sizeof(ts));
    printf("[%s] [%s] MEM PID %d: %s %d MB\n", ts, level_str(level), pid, action, size);
}