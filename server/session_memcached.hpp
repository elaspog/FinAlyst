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

    std::string new_session(User &user);
    void update_session(Session &session);
    void delete_session(Session &session);
    bool load_session(std::string const &sessionid, Session &session);
      
private:
    memcached_st *memcached_conn;
};

#endif
