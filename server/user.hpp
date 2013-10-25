#ifndef USER_HPP_INCLUDED
#define USER_HPP_INCLUDED

#include <iostream>
#include <sstream>
#include <iomanip>
#include <chrono>
#include <thread>
#include <gcrypt.h>

#include "logger.hpp"
#include "database.hpp"
#include "rand.hpp"

class User
{
public:
    typedef unsigned char passtype[128];

    User() :
        _database(NULL), _invalid(true),
        _id(std::numeric_limits<uint64_t>::max())
    {}

    User(Database &database, uint64_t userid) :
        _database(&database),
        _invalid(false), _detached(false), _loaded(false),
        _id(userid)
    {}

    User(Database &database,
            std::string const &name, std::string const &password) :
        _database(&database), _invalid(false), _detached(true),
        _name(name)
    {
        reset_password(password);
        time(&_create);
        _modify = _create;
    }

    Database& database() { return *_database; }
    uint64_t id() const { return _id; }
    void id(uint64_t id) { _id = id; }
    std::string name() { lazy_load(); return _name; }
    void name(std::string const &name) { _name = name; }
    bool administrator() const { return !_invalid && _id == 0; }

    void reset_password(std::string const &password)
    {
        randgen_ret_type salt[sizeof(_passsalt)/2/sizeof(randgen_ret_type)];
        // Generate salt
        for (unsigned i = 0; i < sizeof(salt)/sizeof(salt[0]); ++i)
            salt[i] = secure_rand();
        // Convert to hex
        std::stringstream ss;
        ss << std::hex << std::setfill('0');
        for (unsigned i = 0; i < sizeof(salt)/sizeof(salt[0]); ++i)
            ss << std::setw(sizeof(randgen_ret_type)*2) << salt[i];
        memcpy(_passsalt, ss.str().c_str(), ss.str().length());
        // Generate digest
        digest_pass(password, salt, sizeof(salt), &_passdigest);
    }

    void save()
    {
        if (_detached)
        {
            MYSQL_STMT* stmt = _database->statement(
                    "INSERT INTO `users` (`create`, `modify`, `name`, `passdigest`, `passsalt`) "
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
                mbind(_name, size[2]),
                mbind_fixed((char*)_passdigest, size[3], sizeof(_passdigest)),
                mbind_fixed((char*)_passsalt, size[4], sizeof(_passsalt)),
            };
            static_assert(sizeof(size)/sizeof(size[0]) ==
                sizeof(param)/sizeof(param[0]),
                "param and size array should have the same length");
            
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

    bool authenticate(std::string const &password) const
    {
        // Additional protection agains timing attacks
        std::this_thread::sleep_for(std::chrono::milliseconds(rand()%100));
        if (_invalid) return false;
        passtype passdigest;
        digest_pass(password, &_passsalt, &passdigest);

        std::string s1((char*)_passdigest, sizeof(passtype));
        std::string s2((char*)passdigest, sizeof(passtype));

        /*LOG_DEBUG("Expected: %s", s1.c_str());
        LOG_DEBUG("Hash got: %s", s2.c_str());*/

        // Compare passwords (protected against timeing attack)
        unsigned char same = 0;
        for (unsigned i = 0; i < sizeof(_passdigest); ++i)
            same |= passdigest[i] ^ _passdigest[i];

        return same == 0;
    }

    static User find(Database &database, uint64_t id)
    {
        if (id == 0)
        {
            User root;
            root.name("root");
            root.id(0);
            root._invalid = false;
            return root;
        }
        MYSQL_STMT* stmt = database.statement(
                    "SELECT `id`, `create`, `modify`, `name`, `passdigest`, `passsalt` "
                    " FROM `users` WHERE id = ?");
        if (stmt == NULL)
            throw std::logic_error("Can't create statement");
        unsigned long size[6];
        uint64_t qid = 0;
        MYSQL_TIME create;
        MYSQL_TIME modify;
        char qname[64+1];
        unsigned char qpassdigest[128+1];
        unsigned char qpasssalt[128+1];
        
        MYSQL_BIND result[6] = {
            mbind(qid, size[0]),
            mbind(create, size[1]),
            mbind(modify, size[2]),
            mbind_fixed((char*)qname, size[3], sizeof(qname)),
            mbind_fixed((char*)qpassdigest, size[4], sizeof(qpassdigest)),
            mbind_fixed((char*)qpasssalt, size[5], sizeof(qpasssalt)),
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
        if (err != 0 && err != MYSQL_NO_DATA)
        {
            throw std::logic_error(
                    std::string("Can't fetch statement: ")
                    +mysql_stmt_error(stmt));
        }
        if (err == 0)
        {
            log_assert_equal(id, qid);
            // Name is uniq so we should't get more than one user
            log_assert_equal(mysql_stmt_fetch(stmt), MYSQL_NO_DATA);

            // Make sure all the results fetched
            while (mysql_stmt_fetch(stmt) == 0);

            return User(database, qid,
                    mysql_to_time(create), mysql_to_time(modify),
                    std::string(qname),
                    (passtype*)qpassdigest, (passtype*)qpasssalt);
        }
        return User();
    }

    static User find_by_name(Database &database, std::string const &name)
    {
        MYSQL_STMT* stmt = database.statement(
                    "SELECT `id`, `create`, `modify`, `name`, `passdigest`, `passsalt` "
                    " FROM `users` WHERE name = ?");
        if (stmt == NULL)
            throw std::logic_error("Can't create statement");
        unsigned long size[6];
        uint64_t qid = 0;
        MYSQL_TIME create;
        MYSQL_TIME modify;
        char qname[64+1];
        unsigned char qpassdigest[128+1];
        unsigned char qpasssalt[128+1];
        
        MYSQL_BIND result[6] = {
            mbind(qid, size[0]),
            mbind(create, size[1]),
            mbind(modify, size[2]),
            mbind_fixed((char*)qname, size[3], sizeof(qname)),
            mbind_fixed((char*)qpassdigest, size[4], sizeof(qpassdigest)),
            mbind_fixed((char*)qpasssalt, size[5], sizeof(qpasssalt)),
        };
        static_assert(sizeof(size)/sizeof(size[0]) ==
                sizeof(result)/sizeof(result[0]),
                "result and size array should have the same length");

        unsigned long psize[1];
            MYSQL_BIND param[1] = {
                mbind(name, psize[0])
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
        if (err != 0 && err != MYSQL_NO_DATA)
        {
            throw std::logic_error(
                    std::string("Can't fetch statement: ")
                    +mysql_stmt_error(stmt));
        }
        if (err == 0)
        {
            log_assert_equal(name, qname);
            // Name is uniq so we should't get more than one user
            log_assert_equal(mysql_stmt_fetch(stmt), MYSQL_NO_DATA);

            // Make sure all the results fetched
            while (mysql_stmt_fetch(stmt) == 0);

            return User(database, qid,
                    mysql_to_time(create), mysql_to_time(modify),
                    std::string(qname),
                    (passtype*)qpassdigest, (passtype*)qpasssalt);
        }
        return User();
    }

    static void find_all(Database &database, std::vector<User> &users)
    {
        MYSQL_STMT* stmt = database.statement(
                    "SELECT `id`, `create`, `modify`, `name`, `passdigest`, `passsalt` "
                    " FROM `users`");
        if (stmt == NULL)
            throw std::logic_error("Can't create statement");
        unsigned long size[6];
        uint64_t id = 0;
        MYSQL_TIME create;
        MYSQL_TIME modify;
        char name[64+1];
        unsigned char passdigest[128+1];
        unsigned char passsalt[128+1];
        
        MYSQL_BIND result[6] = {
            mbind(id, size[0]),
            mbind(create, size[1]),
            mbind(modify, size[2]),
            mbind_fixed((char*)name, size[3], sizeof(name)),
            mbind_fixed((char*)passdigest, size[4], sizeof(passdigest)),
            mbind_fixed((char*)passsalt, size[5], sizeof(passsalt)),
        };
        static_assert(sizeof(size)/sizeof(size[0]) ==
                sizeof(result)/sizeof(result[0]),
                "result and size array should have the same length");
        
        if (mysql_stmt_bind_result(stmt, result))
            throw std::logic_error(
                    std::string("Can't bind result in statement: ")
                    +mysql_stmt_error(stmt));
        if (mysql_stmt_execute(stmt))
            throw std::logic_error(
                    std::string("Can't execute statement: ")
                    +mysql_stmt_error(stmt));
        while (mysql_stmt_fetch(stmt) == 0)
        {
            users.push_back(User(database, id,
                    mysql_to_time(create), mysql_to_time(modify),
                    std::string(name),
                    (passtype*)passdigest, (passtype*)passsalt));
        }
    }

    static bool table_exists(Database &database) { return database.table_exists("users"); }

private:
    User(Database &database, uint64_t id,
            time_t create, time_t modify, std::string name,
            passtype const *passdigest, passtype *passsalt) :
        _database(&database), _invalid(false), _detached(false), _loaded(true),
        _id(id), _create(create), _modify(modify), _name(name)
    {
        memcpy(_passdigest, passdigest, sizeof(_passdigest));
        memcpy(_passsalt, passsalt, sizeof(_passsalt));
    }

    static void digest_pass(std::string const &password,
            void const *salt, size_t salt_size,
            passtype *passdigest_)
    {
        unsigned char passdigest[sizeof(passtype)/2];
        // Digest key
        gcry_kdf_derive(password.c_str(), password.length(),
                GCRY_KDF_PBKDF2, GCRY_MD_SHA512,
                (void*)salt, salt_size, 8,
                sizeof(passdigest), (void*)passdigest);
        static_assert(sizeof(passdigest) == 64,
                "passdigest should be 64 bytes");
        // Convert to hex
        std::ostringstream ss;
        ss << std::hex << std::setfill('0');
        for (unsigned i = 0; i < sizeof(passdigest)/sizeof(passdigest[0]); ++i)
        {
            ss << std::setw(sizeof(passdigest[0])*2)
                << (unsigned)passdigest[i];
        }
        log_assert_equal(ss.str().length(), sizeof(passtype));
        memcpy(passdigest_, ss.str().c_str(), ss.str().length());
    }

    static void digest_pass(
            std::string const &password_, passtype const *passsalt_,
            passtype *passdigest_)
    {
        randgen_ret_type binsalt[sizeof(passtype)/2/sizeof(randgen_ret_type)];
        for (unsigned i = 0; i < sizeof(passtype)/2/sizeof(randgen_ret_type); ++i)
        {
            std::string c((char*)passsalt_+i*sizeof(randgen_ret_type)*2,
                    sizeof(randgen_ret_type)*2);
            binsalt[i] = strtoul(c.c_str(), NULL, 16);
            //LOG_DEBUG("%s %08X %u", c.c_str(), binsalt[i], binsalt[i]);
        }
        /*for (unsigned i = 0; i < sizeof(binsalt); ++i)
            LOG_DEBUG("%02X", ((unsigned char*)binsalt)[i]);*/
        digest_pass(password_, (void*)binsalt, sizeof(binsalt), passdigest_);
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

    uint64_t _id;
    time_t _create;
    time_t _modify;
    std::string _name;
    passtype _passdigest;
    passtype _passsalt;
};

#endif
