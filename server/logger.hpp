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
void log_target_syslog(std::string const &application_name);
void log_close();
void log_message(LogLevel level, const char * format, ...);


#define LOG_MESSAGE_WARN(...) log_message(LogLevel::Warning, __VA_ARGS__)
#define LOG_MESSAGE_INFO(...) log_message(LogLevel::Informational, __VA_ARGS__)
#define LOG_MESSAGE_DEBUG(...) log_message(LogLevel::Debug, __VA_ARGS__)

#define log_assert(expr)                        \
    do {                                        \
        if (!(expr))                            \
        {                                       \
            LOG_MESSAGE_DEBUG(__FILE__ ":%d assertation falied: " #expr, __LINE__);    \
            abort();                            \
        }                                       \
    } while (false)

#define log_assert_equal(expr1, expr2)          \
    do {                                        \
        auto e1 = expr1;                        \
        auto e2 = expr2;                        \
        if (e1 != e2)                           \
        {                                       \
            LOG_MESSAGE_DEBUG(__FILE__ ":%d assertation falied: " #expr1 " != " #expr2, __LINE__);    \
            abort();                            \
        }                                       \
    } while (false)

#endif
