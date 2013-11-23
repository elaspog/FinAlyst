#include <cstdio>
#include <cstdarg>
#include <cstring>
#include <time.h>
#include <syslog.h>

#include "logger.hpp"

static FILE *logfile = NULL;
static bool usesyslog;
static char appname[64];

void log_target_file(std::string const &filename)
{
    logfile = fopen(filename.c_str(), "w");
    fprintf(logfile, "Logging started...\n");
    fflush(logfile);
    if (logfile == NULL) abort();
}

void log_target_syslog(std::string const &application_name)
{
    setlogmask (LOG_UPTO (LOG_DEBUG));
    memcpy(appname, application_name.c_str(),
            std::min(sizeof(appname), application_name.length()));
    // make sure C string closed
    appname[sizeof(appname)-1] = '\0';
    openlog(appname, LOG_PID | LOG_NDELAY, LOG_USER);
    usesyslog = true;
}

void log_close()
{
    if (logfile != NULL) fclose(logfile);
    if (usesyslog) closelog(); 
}

void log_message(LogLevel level, const char * format, ...)
{
    if (logfile != NULL)
    {
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
    if (usesyslog)
    {
        va_list args;
        va_start (args, format);
        vsyslog((int)level, format, args);
        va_end (args);
    }
}
