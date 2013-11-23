#ifndef SESSION_MEMCACHED_HPP_INCLUDED
#define SESSION_MEMCACHED_HPP_INCLUDED

#include <libmemcached/memcached.h>

#include "session.hpp"

struct MemcacheError : std::logic_error
{
    MemcacheError(memcached_st *memcached_conn, memcached_return rc) :
        std::logic_error(memcached_strerror(memcached_conn, rc))
    {}
};

class MemcachedSessionManager : public SessionManager
{
public:
    MemcachedSessionManager(Database &database, std::string const &config) :
        SessionManager(database)
    {
        memcached_conn = memcached(config.data(), config.length());
    }

    ~MemcachedSessionManager()
    {
        memcached_free(memcached_conn);
    }

    std::string new_session(User &user)
    {
        std::string sessionid = generate_new_sessionid(user);
        Session session(user, sessionid);
        update_session(session);
        return sessionid;
    }

    void update_session(Session &session)
    {
        memcached_return rc;
        std::string const &sessionid = session.sessionid();
        std::ostringstream ss;
        save_pairs(ss, '\n', session.data());
        std::string value("userid="+std::to_string(session.user().id()));
        value += ss.str();
        rc = memcached_set(memcached_conn,
                sessionid.data(), sessionid.length(),
                value.data(), value.length(), 
                session_timeout(), 0);
        if (rc != MEMCACHED_SUCCESS)
            throw MemcacheError(memcached_conn, rc);
    }

    void delete_session(Session &session)
    {
        memcached_return rc;
        std::string const &sessionid = session.sessionid();
        rc = memcached_delete(memcached_conn, sessionid.data(),
                sessionid.length(), 0);
        if (rc != MEMCACHED_SUCCESS)
            throw MemcacheError(memcached_conn, rc);
    }

    bool load_session(std::string const &sessionid, Session &session)
    {
        memcached_return_t rc;
        size_t value_length;
        char *value = memcached_get(memcached_conn,
                sessionid.data(), sessionid.length(),
                &value_length, 0, &rc);
        if (memcached_success(rc))
        {
            std::stringstream valuestream(std::string(value, value_length));
            parse_pairs(valuestream, '\n', session.data());
            uint64_t userid = 0;
            try {
                userid = std::stoi(session.get("userid"));
            } catch (std::exception const &ex)
            {
                LOG_MESSAGE_WARN("Invalid userid in session data");
                return false;
            }
            session.set(User::find(database(), userid), sessionid);
        }
        if (value != NULL) free(value);
        return memcached_success(rc);
    }
   
private:
    memcached_st *memcached_conn;
};

#endif
