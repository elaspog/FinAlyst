#include <cstdio>
#include <cstdarg>
#include <time.h>

#include "logger.hpp"

static FILE *logfile = NULL;

void log_target_file(std::string const &filename)
{
    logfile = fopen(filename.c_str(), "w");
    fprintf(logfile, "Logging started...\n");
    fflush(logfile);
    if (logfile == NULL) abort();
}

void log_message(LogLevel level, const char * format, ...)
{
    if (logfile == NULL) return;
    (void)level; // TODO: level ignored
    char timebuff[64];
    time_t t;
    time(&t);
    struct tm brokendown;
    localtime_r(&t, &brokendown);
    strftime(timebuff, sizeof(timebuff), "%Y-%m-%d %H:%M:%S%z", &brokendown);
    fprintf(logfile, "[%s] ", timebuff);
    va_list args;
    va_start (args, format);
    vfprintf (logfile, format, args);
    va_end (args);
    fprintf(logfile, "\n");
    fflush(logfile);
}
