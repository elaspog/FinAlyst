#ifndef DATABASE_HPP_INCLUDED
#define DATABASE_HPP_INCLUDED

#include <map>
#include <string>
#include <stdexcept>
#include <cstring>
#include <time.h>
#include <mysql/mysql.h>

class Database
{
public:
    Database(std::string const &server, std::string const &user,
            std::string const &pass, std::string const &database)
    {
        // connect to database
        _mysql = mysql_init(NULL);
        if (_mysql == NULL) 
        {
            _error_message = "Can't initialize mysql: ";
            _error_message += mysql_error(_mysql);
        } else
        if (mysql_real_connect(_mysql, server.c_str(), user.c_str(),
                    pass.c_str(), database.c_str(),
                    0, NULL, 0) == NULL) 
        {
            _error_message = "Can't connect to database: ";
            _error_message += mysql_error(_mysql);
        } else _connected = true;
    }

    ~Database()
    {
        // Make sure all the prepared statements closed.
        // Ignore errors, we need to close them anyway, and it is
        // also bad practice to throw exception in dtor.
        for (auto &it : statements) mysql_stmt_close(it.second);
        // close database connection
        mysql_close(_mysql);
    }

    Database(Database const&) = delete;
    Database& operator=(Database const&) = delete;

    bool connected() { return _connected; }

    std::string error_message() { return _error_message; }

    bool table_exists(std::string const &name)
    {
        return mysql_query(_mysql, ("SELECT count(*) from `"+name+"`").c_str()) == 0;
    }

    MYSQL_STMT* statement(std::string const &query)
    {
        auto it = statements.find(query);
        if (it == statements.end())
        {
            MYSQL_STMT *stmt = mysql_stmt_init(_mysql);
            if (!stmt) throw std::logic_error("Prepare statement init falied!");
            if (mysql_stmt_prepare(stmt, query.c_str(), query.length()))
            {
                throw std::logic_error(std::string("Prepare statement failed: ")+
                        +mysql_stmt_error(stmt));
            }
            statements[query] = stmt;
            return stmt;
        }
        return it->second;
    }

private:
    std::map<std::string, MYSQL_STMT*> statements;
    bool _connected;
    MYSQL *_mysql;
    std::string _error_message;
};

inline time_t mysql_to_time(MYSQL_TIME &dt)
{
    struct tm bt;
    // TODO: unhandled
    log_assert_equal(dt.neg, false);
    bt.tm_hour = dt.hour;
    bt.tm_min = dt.minute;
    bt.tm_sec = dt.second;
    bt.tm_year = dt.year-1900;
    bt.tm_mon = dt.month;
    bt.tm_mday = dt.day;
    return mktime(&bt);
}

inline void time_to_mysql(time_t t, MYSQL_TIME &dt)
{
    memset(&dt, 0, sizeof(dt));
    struct tm bt;
    localtime_r(&t, &bt);
    dt.hour = bt.tm_hour;
    dt.minute = bt.tm_min;
    dt.second = bt.tm_sec;
    dt.year = bt.tm_year+1900;
    dt.month = bt.tm_mon;
    dt.day = bt.tm_mday;
    dt.neg = false;
}

inline std::string time_to_string(time_t t)
{
    struct tm bt;
    char buffer[128];
    localtime_r(&t, &bt);
    strftime(buffer, sizeof(buffer), "%F %T", &bt);
    return std::string(buffer);
}

inline MYSQL_BIND mbind_fixed(char const *str, unsigned long &size, size_t fixed_length)
{
    size = fixed_length;
    MYSQL_BIND bind;
    memset(&bind, 0, sizeof(bind));
    bind.buffer_type = MYSQL_TYPE_STRING;
    bind.buffer = (void*)str;
    bind.buffer_length = size;
    bind.length = &size;
    bind.is_null = 0;
    return bind;
}

inline MYSQL_BIND mbind(std::string const &str, unsigned long &size)
{
    size = str.length();
    MYSQL_BIND bind;
    memset(&bind, 0, sizeof(bind));
    bind.buffer_type = MYSQL_TYPE_STRING;
    bind.buffer = (void*)str.c_str();
    bind.buffer_length = str.length();
    bind.length = &size;
    bind.is_null = 0;
    return bind;
}

inline MYSQL_BIND mbind(uint64_t const &num, unsigned long &size)
{
    size = sizeof(num);
    MYSQL_BIND bind;
    memset(&bind, 0, sizeof(bind));
    bind.buffer_type = MYSQL_TYPE_LONGLONG;
    bind.buffer = (void*)&num;
    bind.buffer_length = size;
    bind.length = &size;
    bind.is_null = 0;
    return bind;
}

inline MYSQL_BIND mbind(MYSQL_TIME const &datetime, unsigned long &size)
{
    size = sizeof(datetime);
    MYSQL_BIND bind;
    memset(&bind, 0, sizeof(bind));
    bind.buffer_type = MYSQL_TYPE_DATETIME;
    bind.buffer = (void*)&datetime;
    bind.buffer_length = size;
    bind.length = &size;
    bind.is_null = 0;
    return bind;
}


#endif
