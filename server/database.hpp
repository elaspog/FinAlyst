#ifndef DATABASE_HPP_INCLUDED
#define DATABASE_HPP_INCLUDED

#include <map>
#include <string>
#include <stdexcept>
#include <cstring>
#include <time.h>
#include <functional>
#include <mysql/mysql.h>

#include "logger.hpp"

/*class Statement
{
    MYSQL_STMT *stmt;

    ~Statement()
    {
        // Ignore errors, we need to close it anyway, and it is
        // also bad practice to throw exception in dtor.
        mysql_stmt_close(stmt);
    }
};*/

struct RowDoesNotExists : std::invalid_argument
{
    RowDoesNotExists(std::string const &message) :
        std::invalid_argument(message)
    {}
};

template <size_t size>
class FixedString
{
    char _c_str[size+1];

public:
    FixedString() { _c_str[0] = 0; }
    FixedString(std::string const &str)
    {
        memcpy(_c_str, str.c_str(), str.length()+1);
    }
    char const* c_str() const { return _c_str; }
    char* c_str() { return _c_str; }
    size_t length() const { return strlen(_c_str); }
    std::string to_string()
    {
        return std::string(_c_str);
    }
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

template <size_t Size>
MYSQL_BIND mbind(FixedString<Size> const &str, unsigned long &size)
{
    size = str.length();
    LOG_DEBUG("Lemngth: %d", size);
    MYSQL_BIND bind;
    memset(&bind, 0, sizeof(bind));
    bind.buffer_type = MYSQL_TYPE_STRING;
    bind.buffer = (void*)str.c_str();
    bind.buffer_length = Size;
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

class Database
{
    template <class BindTuple, size_t N>
    struct SetupBind
    {
        static void setup(BindTuple const &tuple, unsigned long *size, MYSQL_BIND *bind)
        {
            SetupBind<BindTuple, N-1>::setup(tuple, size-1, bind-1);
            //LOG_DEBUG("bind param to %X", (uint64_t)bind);
            *bind = mbind(std::get<N-1>(tuple), *size);
        }
    };

    template <class BindTuple>
    struct SetupBind<BindTuple, 1>
    {
        static void setup(BindTuple const &tuple, unsigned long *size, MYSQL_BIND *bind)
        {
            //LOG_DEBUG("bind last param to %X", (uint64_t)bind);
            *bind = mbind(std::get<0>(tuple), *size);
        }
    };

    template <class BindTuple>
    struct SetupBind<BindTuple, 0>
    {
        static void setup(BindTuple const &tuple, unsigned long *size, MYSQL_BIND *bind)
        {
            (void)tuple; (void)size; (void)bind;
        }
    };

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

    template <class Params, class Results,
             typename ResultCallback = std::function< void (Results &res)>>
    void query(Params const &param_values, std::string const &query, ResultCallback cb)
    {
        // Get statement
        MYSQL_STMT *stmt = statement(query);
        // Setup query arguments
        //Params param_values;
        unsigned long param_size[std::tuple_size<Params>::value];
        MYSQL_BIND params[std::tuple_size<Params>::value];
        if (std::tuple_size<Params>::value > 0)
        {
            SetupBind<Params, std::tuple_size<Params>::value>::setup(
                    param_values,
                    param_size+std::tuple_size<Params>::value-1,
                    params+std::tuple_size<Params>::value-1);
            
            if (mysql_stmt_bind_param(stmt, params))
                throw std::logic_error(
                        std::string("Can't bind param in statement: ")
                        +mysql_stmt_error(stmt));
        }
        // Setup result set
        Results res_values;
        unsigned long res_size[std::tuple_size<Results>::value];
        MYSQL_BIND results[std::tuple_size<Results>::value];
        if (std::tuple_size<Results>::value > 0)
        {
            LOG_DEBUG("Results: %d", std::tuple_size<Results>::value);
            SetupBind<Results, std::tuple_size<Results>::value>::setup(
                    res_values,
                    res_size+std::tuple_size<Results>::value-1,
                    results+std::tuple_size<Results>::value-1);
            if (mysql_stmt_bind_result(stmt, results))
                throw std::logic_error(
                        std::string("Can't bind result in statement: ")
                        +mysql_stmt_error(stmt));
        }

        // Execute query
        if (mysql_stmt_execute(stmt))
            throw std::logic_error(
                    std::string("Can't execute statement: ")
                    +mysql_stmt_error(stmt));
        // Fetch data
        /*for (unsigned i = 0; i<std::tuple_size<Results>::value;++i)
                LOG_DEBUG("%d size: %d", i, res_size[i]);*/
        if (std::tuple_size<Results>::value > 0)
        {
            int error;
            while ((error = mysql_stmt_fetch (stmt)) == 0)
            {
                LOG_DEBUG("Got result");
                /*for (unsigned i = 0; i<std::tuple_size<Results>::value;++i)
                    LOG_DEBUG("%d got size: %d", i, res_size[i]);*/
                cb(res_values);
            }
            if (error != MYSQL_NO_DATA)
            {
                LOG_DEBUG("fetch error: %d", error);
                throw std::logic_error(
                        std::string("Can't fetch: ")
                        +mysql_stmt_error(stmt));
            }
        }
    }

    template <class Params>
    void execute(Params const &param_values, std::string const &query)
    {
        typedef std::tuple<> Results;
        Results result_values;
        this->query<Params, Results>(param_values, query,
                [] (Results &res) {
                    (void)res;
                    log_assert(false);
                });
    }

    uint64_t last_insert_id()
    {
        return mysql_insert_id(_mysql);
    }

    uint64_t affected_rows()
    {
        return mysql_affected_rows(_mysql);
    }

private:

    std::map<std::string, MYSQL_STMT*> statements;
    bool _connected;
    MYSQL *_mysql;
    std::string _error_message;
};

#endif
