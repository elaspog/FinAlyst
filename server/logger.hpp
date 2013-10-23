#ifndef LOGGER_HPP_INCLUDED
#define LOGGER_HPP_INCLUDED

#include <string>

enum class LogLevel
{
    Emergency           = 0,
    Alert               = 1,
    Critical            = 2,
    Error               = 3,
    Warning             = 4,
    Notice              = 5,
    Informational       = 6,
    Debug               = 7
};

void log_target_file(std::string const &filename);
void log_message(LogLevel level, const char * format, ...);

#define LOG_DEBUG(...) log_message(LogLevel::Debug, __VA_ARGS__);

#endif
