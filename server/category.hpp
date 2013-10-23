#ifndef CATEGORY_HPP_INCLUDED
#define CATEGORY_HPP_INCLUDED

#include <string>
#include <vector>

#include "database.hpp"
#include "user.hpp"

class Category
{
public:

    Category() :
        _database(NULL), _invalid(true),
        _id(std::numeric_limits<uint64_t>::max())
    {}

    Category(Database &database, uint64_t categoryid) :
        _database(&database),
        _invalid(false), _detached(false), _loaded(false),
        _id(categoryid)
    {}

    Category(Database &database, User const &user,
            std::string const &name, std::string description) :
        _database(&database),
        _invalid(false), _detached(true), _loaded(true),
        _userid(user.id()), _name(name), _description(description)
    {
        time(&_create);
        _modify = _create;
    }

    User user() { return User(*_database, _userid); }

    bool valid() { return !_invalid; }
    uint64_t id() const { return _id; }
    time_t create() { return _create; }
    time_t modify() { return _modify; }
    std::string name() { lazy_load(); return _name; }
    std::string description() { lazy_load(); return _description; }

    void save()
    {
        if (_detached)
        {
            MYSQL_STMT* stmt = _database->statement(
                    "INSERT INTO `categories` (`create`, `modify`, `userid`, `name`, `description`) "
                    "VALUES (?, ?, ?, ?, ?)");
            if (stmt == NULL)
                throw std::logic_error("Can't create statement");
            unsigned long size[5];

            MYSQL_TIME create;
            MYSQL_TIME modify;
            time_to_mysql(_create, create);
            time_to_mysql(_modify, modify);

            MYSQL_BIND param[5] = {
                mbind(create, size[0]),
                mbind(modify, size[1]),
                mbind(_userid, size[2]),
                mbind(_name, size[3]),
                mbind(_description, size[4]),
            };
            
            if (mysql_stmt_bind_param(stmt, param))
                throw std::logic_error(
                        std::string("Can't bind param in statement: ")
                        +mysql_stmt_error(stmt));
            if (mysql_stmt_execute(stmt))
                throw std::logic_error(
                        std::string("Can't execute statement: ")
                        +mysql_stmt_error(stmt));
        } else
        {
            // TODO: do update
        }
    }

    static Category find(Database &database, uint64_t id)
    {
        MYSQL_STMT* stmt = database.statement(
                    "SELECT `id`, `create`, `modify`, `userid`, `name`, `description` "
                    " FROM `categories` WHERE id = ?");
        if (stmt == NULL)
            throw std::logic_error("Can't create statement");
        
        unsigned long size[6];
        uint64_t qid = 0;
        MYSQL_TIME create;
        MYSQL_TIME modify;
        uint64_t quserid;
        char qname[256+1];
        char qdesc[256+1];
        
        MYSQL_BIND result[6] = {
            mbind(qid, size[0]),
            mbind(create, size[1]),
            mbind(modify, size[2]),
            mbind(quserid, size[3]),
            mbind_fixed((char*)qname, size[4], sizeof(qname)),
            mbind_fixed((char*)qdesc, size[5], sizeof(qdesc)),
        };
        static_assert(sizeof(size)/sizeof(size[0]) ==
                sizeof(result)/sizeof(result[0]),
                "result and size array should have the same length");

        unsigned long psize[1];
            MYSQL_BIND param[1] = {
                mbind(id, psize[0])
            };
        
        if (mysql_stmt_bind_param(stmt, param))
                throw std::logic_error(
                        std::string("Can't bind param in statement: ")
                        +mysql_stmt_error(stmt));
        if (mysql_stmt_bind_result(stmt, result))
            throw std::logic_error(
                    std::string("Can't bind result in statement: ")
                    +mysql_stmt_error(stmt));
        if (mysql_stmt_execute(stmt))
            throw std::logic_error(
                    std::string("Can't execute statement: ")
                    +mysql_stmt_error(stmt));
        // Fetch data
        auto err = mysql_stmt_fetch(stmt);
        log_assert(err == 0 || err == MYSQL_NO_DATA);
        if (err == 0)
        {
            log_assert_equal(id, qid);
            // Name is uniq so we should't get more than one result
            log_assert_equal(mysql_stmt_fetch(stmt), MYSQL_NO_DATA);

            // Make sure all the results fetched
            while (mysql_stmt_fetch(stmt) == 0);

            return Category(database, qid,
                    mysql_to_time(create), mysql_to_time(modify),
                    quserid, std::string(qname), std::string(qdesc));
        }
        return Category();
    }

    static void find_all(Database &database,
            User const &user, std::vector<Category> &categories)
    {
        MYSQL_STMT* stmt = database.statement(
                    "SELECT `id`, `create`, `modify`, `name`, `description` "
                    " FROM `categories` WHERE userid = ?");
        if (stmt == NULL)
            throw std::logic_error("Can't create statement");
        unsigned long size[5];
        uint64_t qid = 0;
        MYSQL_TIME create;
        MYSQL_TIME modify;
        char qname[64+1];
        char qdesc[256+1];
        
        MYSQL_BIND result[5] = {
            mbind(qid, size[0]),
            mbind(create, size[1]),
            mbind(modify, size[2]),
            mbind_fixed((char*)qname, size[3], sizeof(qname)),
            mbind_fixed((char*)qdesc, size[4], sizeof(qdesc)),
        };
        static_assert(sizeof(size)/sizeof(size[0]) ==
                sizeof(result)/sizeof(result[0]),
                "result and size array should have the same length");

        uint64_t userid = user.id();
        unsigned long psize[1];
        MYSQL_BIND param[1] = {
            mbind(userid, psize[0])
        };
        
        if (mysql_stmt_bind_param(stmt, param))
                throw std::logic_error(
                        std::string("Can't bind param in statement: ")
                        +mysql_stmt_error(stmt));
        if (mysql_stmt_bind_result(stmt, result))
            throw std::logic_error(
                    std::string("Can't bind param in statement: ")
                    +mysql_stmt_error(stmt));
        if (mysql_stmt_execute(stmt))
            throw std::logic_error(
                    std::string("Can't execute statement: ")
                    +mysql_stmt_error(stmt));
        while (mysql_stmt_fetch (stmt) == 0)
        {
            categories.push_back(Category(database, qid,
                        mysql_to_time(create), mysql_to_time(modify),
                        user.id(),
                        std::string(qname), std::string(qdesc)));
        }
    }

private:

    Category(Database &database, uint64_t id,
            time_t create, time_t modify, uint64_t userid,
            std::string const &name, std::string description) :
        _database(&database),
        _invalid(false), _detached(false), _loaded(true),
        _id(id), _create(create), _modify(modify),
        _userid(userid), _name(name), _description(description)
    {}

    void lazy_load()
    {
        if (_loaded) return;
        // TODO
        log_assert(false);
    }

    Database *_database;
    bool _invalid;
    bool _detached;
    bool _loaded;

    uint64_t _id;
    time_t _create;
    time_t _modify;
    uint64_t _userid;
    std::string _name;
    std::string _description;
};

#endif
