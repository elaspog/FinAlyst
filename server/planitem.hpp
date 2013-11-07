#ifndef PLAN_HPP_INCLUDED
#define PLAN_HPP_INCLUDED

#include <string>
#include <vector>

#include "database.hpp"
#include "user.hpp"
#include "category.hpp"

class PlanItem
{
public:

    PlanItem() :
        _database(NULL), _invalid(true),
        _id(std::numeric_limits<uint64_t>::max())
    {}

    PlanItem(Database &database, User const &user, Category const &category,
            uint64_t amount, std::string description) :
        _database(&database), _invalid(false), _detached(true),
        _userid(user.id()), _categoryid(category.id()),
        _amount(amount), _description(description)
    {
        time(&_create);
        _modify = _create;
    }

    Category category() { return Category(*_database, _categoryid); }
    User user() { return User(*_database, _userid); }

    bool valid() { return !_invalid; }
    uint64_t id() const { return _id; }
    time_t create() { return _create; }
    time_t modify() { return _modify; }
    uint64_t amount() { return _amount; }
    std::string description() { return _description; }

    void save()
    {
        if (_detached)
        {
            MYSQL_STMT* stmt = _database->statement(
                    "INSERT INTO `plan` (`create`, `modify`, `userid`, `categoryid`, `amount`, `description`) "
                    "VALUES (?, ?, ?, ?, ?, ?)");
            if (stmt == NULL)
                throw std::logic_error("Can't create statement");
            unsigned long size[6];

            MYSQL_TIME create;
            MYSQL_TIME modify;
            time_to_mysql(_create, create);
            time_to_mysql(_modify, modify);

            MYSQL_BIND param[6] = {
                mbind(create, size[0]),
                mbind(modify, size[1]),
                mbind(_userid, size[2]),
                mbind(_categoryid, size[3]),
                mbind(_amount, size[4]),
                mbind(_description, size[5]),
            };
            
            if (mysql_stmt_bind_param(stmt, param))
                throw std::logic_error(
                        std::string("Can't bind param in statement: ")
                        +mysql_stmt_error(stmt));
            if (mysql_stmt_execute(stmt))
                throw std::logic_error(
                        std::string("Can't execute statement: ")
                        +mysql_stmt_error(stmt));
            _id = _database->last_insert_id();
        } else
        {
            // TODO: do update
        }
    }

    static PlanItem find(Database &database, uint64_t id)
    {
        typedef std::tuple<uint64_t> Params;
        Params params = std::make_tuple(id);
        return query_uniqe(database, params,
                "SELECT `id`, `create`, `modify`, `userid`, `categoryid`, `amount`, `description` "
                    " FROM `items` WHERE id = ?");
    }

    static void find_all(Database &database,
            User const &user, std::vector<PlanItem> &items)
    {
        typedef std::tuple<uint64_t> Params;
        Params params = std::make_tuple(user.id());
        query(database, params, items,
                "SELECT `id`, `create`, `modify`, `userid`, `categoryid`, `amount`, `description` "
                    " FROM `items` WHERE userid = ?");
    }

    /*static void find_all(Database &database,
            User const &user, std::vector<Plan> &plans)
    {
        MYSQL_STMT* stmt = database.statement(
                    "SELECT `id`, `create`, `modify`, `amount`, `description` "
                    " FROM `plan` WHERE userid = ?");
        if (stmt == NULL)
            throw std::logic_error("Can't create statement");
        unsigned long size[5];
        uint64_t qid = 0;
        MYSQL_TIME create;
        MYSQL_TIME modify;
        uint64_t amount;
        char qdesc[256+1];
        
        MYSQL_BIND result[5] = {
            mbind(qid, size[0]),
            mbind(create, size[1]),
            mbind(modify, size[2]),
            mbind(amount, size[3]),
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
            plans.push_back(Plan(database, qid,
                        mysql_to_time(create), mysql_to_time(modify),
                        user.id(),
                        amount, std::string(qdesc)));
        }
    }*/

private:

    PlanItem(Database &database, uint64_t id,
            time_t create, time_t modify, 
            uint64_t userid, uint64_t categoryid,
            uint64_t amount, std::string description) :
        _database(&database), _detached(false), _id(id),
        _create(create), _modify(modify),
        _userid(userid), _categoryid(categoryid),
        _amount(amount), _description(description)
    {}

    template <typename Params>
    static PlanItem query_uniqe(Database &database, Params const &params,
            std::string const &query)
    {
        typedef std::tuple<uint64_t, MYSQL_TIME, MYSQL_TIME,
                uint64_t, uint64_t, uint64_t, FixedString<256>> Results;
        PlanItem item;
        database.query<Params, Results>(params, query,
                [&database, &item] (Results &res) {
                    log_assert(!item.valid());
                    item = PlanItem(database,
                        std::get<0>(res),
                        mysql_to_time(std::get<1>(res)),
                        mysql_to_time(std::get<2>(res)),
                        std::get<3>(res),
                        std::get<4>(res),
                        std::get<5>(res),
                        std::get<6>(res).to_string());
                });
        return item;
    }

    template <typename Params>
    static void query(Database &database, Params const &params,
            std::vector<PlanItem> &items, std::string const &query)
    {
        typedef std::tuple<uint64_t, MYSQL_TIME, MYSQL_TIME,
                uint64_t, uint64_t, uint64_t, FixedString<256>> Results;
        database.query<Params, Results>(params, query,
                [&database, &items] (Results &res) {
                    items.push_back( PlanItem(database,
                        std::get<0>(res),
                        mysql_to_time(std::get<1>(res)),
                        mysql_to_time(std::get<2>(res)),
                        std::get<3>(res),
                        std::get<4>(res),
                        std::get<5>(res),
                        std::get<6>(res).to_string()));
                });
    }

    Database *_database;
    bool _invalid;
    bool _detached;

    uint64_t _id;
    time_t _create;
    time_t _modify;
    uint64_t _userid;
    uint64_t _categoryid;
    uint64_t _amount;
    std::string _description;
};

#endif
