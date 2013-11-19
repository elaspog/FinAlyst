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
        _database(NULL),
        _invalid(true), _detached(true), _loaded(false), _changed(false),
        _id(std::numeric_limits<uint64_t>::max())
    {}

    PlanItem(Database &database, User const &user, Category const &category,
            uint64_t amount, std::string description) :
        _database(&database),
        _invalid(false), _detached(true), _loaded(true), _changed(true),
        _userid(user.id()), _categoryid(category.id()),
        _amount(amount), _description(description)
    {
        time(&_create);
        _modify = _create;
    }

    Category category() { return Category(*_database, _categoryid); }
    User user() { return User(*_database, _userid); }

    bool detached() const { return _detached; }
    bool valid() const { return !_invalid; }
    uint64_t id() const { return _id; }
    time_t create() { lazy_load(); return _create; }
    time_t modify() { lazy_load(); return _modify; }
    uint64_t amount() { lazy_load(); return _amount; }
    std::string description() { lazy_load(); return _description; }

    void amount(uint64_t amount)
    {
        _amount = amount;
        _changed = true;
    }

    void description(std::string const &desc)
    {
        _description = desc;
        _changed = true;
    }

    void category(Category &category)
    {
        log_assert(!category.detached() && category.valid());
        _categoryid = category.id();
        _changed = true;
    }

    static void destroy(Database &database, uint64_t userid, uint64_t id)
    {
        typedef std::tuple<uint64_t, uint64_t> Params;
        Params params = std::make_tuple(id, userid);
        database.execute(params,
                "DELETE FROM `planitems` WHERE id = ? and userid = ?");
        uint64_t affected = database.affected_rows();
        LOG_DEBUG("Affected: %d", affected);
        log_assert(affected <= 1);
        if (affected == 0)
            throw RowDoesNotExists("Can't destroy item, user does not "
                    "have that item!");
    }

    static void destroy(Database &database, PlanItem &obj)
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
                    "INSERT INTO `planitems` (`create`, `modify`, `userid`, `categoryid`, `amount`, `description`) "
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
            if (!_changed) return;
            time_t current;
            time(&current);
            MYSQL_TIME newmodify;
            time_to_mysql(current, newmodify);
            typedef std::tuple<MYSQL_TIME, uint64_t,
                    uint64_t, FixedString<256>, uint64_t, uint64_t> Params;
            Params params = std::make_tuple(
                    newmodify,
                    _categoryid,
                    _amount,
                    FixedString<256>(_description), _id, _userid);
            _database->execute(params,
                "UPDATE `planitems` SET `modify` = ?, `categoryid` = ?, "
                " `amount` = ?, `description` = ? "
                " WHERE id = ? AND userid = ?");
            uint64_t affected = _database->affected_rows();
            log_assert(affected <= 1);
            if (affected == 0)
                throw RowDoesNotExists("Can't change item, category does "
                        "not exists!");
        }
    }

    static PlanItem find(Database &database, uint64_t id)
    {
        typedef std::tuple<uint64_t> Params;
        Params params = std::make_tuple(id);
        return query_uniqe(database, params,
                "SELECT `id`, `create`, `modify`, `userid`, `categoryid`, `amount`, `description` "
                    " FROM `planitems` WHERE id = ?");
    }

    static void find_all(Database &database,
            User const &user, std::vector<PlanItem> &items)
    {
        typedef std::tuple<uint64_t> Params;
        Params params = std::make_tuple(user.id());
        query(database, params, items,
                "SELECT `id`, `create`, `modify`, `userid`, `categoryid`, `amount`, `description` "
                    " FROM `planitems` WHERE userid = ?");
    }

    void to_json(std::ostream &output)
    {
        output << "\t\t{\n";
        output << "\t\t\t\"id\": \"" << id() << "\",\n";
        output << "\t\t\t\"create\": \"/Date(" << (uint64_t)create()*1000 << ")/\",\n";
        output << "\t\t\t\"modify\": \"/Date(" << (uint64_t)modify()*1000 << ")/\",\n";
        output << "\t\t\t\"categoryid\": \"" << category().id() << "\",\n";
        output << "\t\t\t\"amount\": \"" << amount() << "\",\n";
        output << "\t\t\t\"description\": \"";
            jsonescape(description(), output);
            output << "\"\n";
        output << "\t\t}";
    }

private:

    PlanItem(Database &database, uint64_t id,
            time_t create, time_t modify, 
            uint64_t userid, uint64_t categoryid,
            uint64_t amount, std::string description) :
        _database(&database),
        _invalid(false), _detached(false), _loaded(true), _changed(false),
        _id(id), _create(create), _modify(modify),
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
    uint64_t _categoryid;
    uint64_t _amount;
    std::string _description;
};

#endif
