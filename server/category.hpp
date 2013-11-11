#ifndef CATEGORY_HPP_INCLUDED
#define CATEGORY_HPP_INCLUDED

#include <string>
#include <vector>
#include <tuple>

#include "database.hpp"
#include "user.hpp"

class Category
{
public:

    Category() :
        _database(NULL),
        _invalid(true), _detached(true), _loaded(false), _changed(false),
        _id(std::numeric_limits<uint64_t>::max())
    {}

    Category(Database &database, uint64_t categoryid) :
        _database(&database),
        _invalid(false), _detached(false), _loaded(false), _changed(false),
        _id(categoryid)
    {}

    Category(Database &database, User const &user,
            std::string const &name, std::string description) :
        _database(&database),
        _invalid(false), _detached(true), _loaded(true), _changed(true),
        _userid(user.id()), _name(name), _description(description)
    {
        time(&_create);
        _modify = _create;
    }

    User user() { return User(*_database, _userid); }

    bool valid() { return !_invalid; }
    uint64_t id() const { return _id; }
    time_t create() { lazy_load(); return _create; }
    time_t modify() { lazy_load(); return _modify; }
    std::string name() { lazy_load(); return _name; }
    std::string description() { lazy_load(); return _description; }

    void name(std::string const &name)
    {
        _name = name;
        _changed = true;
    }

    void description(std::string const &desc)
    {
        _description = desc;
        _changed = true;
    }

    static void destroy(Database &database, uint64_t userid, uint64_t id)
    {
        typedef std::tuple<uint64_t, uint64_t> Params;
        Params params = std::make_tuple(id, userid);
        database.execute(params,
                "DELETE FROM `categories` WHERE id = ? and userid = ?");
        uint64_t affected = database.affected_rows();
        LOG_DEBUG("Affected: %d", affected);
        log_assert(affected <= 1);
        if (affected == 0)
            throw RowDoesNotExists("Can't destroy category, user does not "
                    "have that category!");
    }

    static void destroy(Database &database, Category &obj)
    {
        log_assert(!obj._invalid && !obj._detached);
        destroy(database, obj.user().id(), obj.id());
        obj._detached = true;
    }

    void destroy() { destroy(*_database, *this); }

    void save()
    {
        log_assert(!_invalid);
        log_assert(_loaded);
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
            _id = _database->last_insert_id();
        } else
        {
            time_t current;
            time(&current);
            MYSQL_TIME newmodify;
            time_to_mysql(current, newmodify);
            typedef std::tuple<MYSQL_TIME,
                    FixedString<128>, FixedString<256>, uint64_t, uint64_t> Params;
            Params params = std::make_tuple(
                    newmodify,
                    FixedString<128>(_name),
                    FixedString<256>(_description), _id, _userid);
            _database->execute(params,
                "UPDATE `categories` SET `modify` = ?, "
                " `name` = ?, `description` = ? "
                " WHERE id = ? AND userid = ?");
            uint64_t affected = _database->affected_rows();
            log_assert(affected <= 1);
            if (affected == 0)
                throw RowDoesNotExists("Can't change category, category does "
                        "not exists!");
        }
    }

    static Category find(Database &database, uint64_t id)
    {
        typedef std::tuple<uint64_t> Params;
        Params params = std::make_tuple(id);
        return query_uniqe(database, params,
                "SELECT `id`, `create`, `modify`, `userid`, `name`, `description` "
                " FROM `categories` WHERE id = ?");
    }

    static void find_all(Database &database,
            User const &user, std::vector<Category> &categories)
    {
        typedef std::tuple<uint64_t> Params;
        Params params = std::make_tuple(user.id());
        query(database, params, categories,
                "SELECT `id`, `create`, `modify`, `userid`, `name`, `description` "
                " FROM `categories` WHERE userid = ?");
    }

private:

    Category(Database &database, uint64_t id,
            time_t create, time_t modify, uint64_t userid,
            std::string const &name, std::string const &description) :
        _database(&database),
        _invalid(false), _detached(false), _loaded(true), _changed(false),
        _id(id), _create(create), _modify(modify),
        _userid(userid), _name(name), _description(description)
    {}

    template <typename Params>
    static Category query_uniqe(Database &database, Params const &params,
            std::string const &query)
    {
        typedef std::tuple<uint64_t, MYSQL_TIME, MYSQL_TIME,
                uint64_t, FixedString<128>, FixedString<256>> Results;
        Category category;
        database.query<Params, Results>(params, query,
                [&database, &category] (Results &res) {
                    log_assert(!category.valid());
                    category = Category(database,
                        std::get<0>(res),
                        mysql_to_time(std::get<1>(res)),
                        mysql_to_time(std::get<2>(res)),
                        std::get<3>(res),
                        std::get<4>(res).to_string(),
                        std::get<5>(res).to_string());
                });
        return category;
    }

    template <typename Params>
    static void query(Database &database, Params const &params,
            std::vector<Category> &categories, std::string const &query)
    {
        typedef std::tuple<uint64_t, MYSQL_TIME, MYSQL_TIME,
                uint64_t, FixedString<128>, FixedString<256>> Results;
        database.query<Params, Results>(params, query,
                [&database, &categories] (Results &res) {
                    categories.push_back(Category(database,
                        std::get<0>(res),
                        mysql_to_time(std::get<1>(res)),
                        mysql_to_time(std::get<2>(res)),
                        std::get<3>(res),
                        std::get<4>(res).to_string(),
                        std::get<5>(res).to_string()));
                });
    }

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
    bool _changed;

    uint64_t _id;
    time_t _create;
    time_t _modify;
    uint64_t _userid;
    std::string _name;
    std::string _description;
};

#endif
