#ifndef SESSION_MEM_HPP_INCLUDED
#define SESSION_MEM_HPP_INCLUDED

#include "session.hpp"

class MemorySessionManager : public SessionManager
{
public:
    MemorySessionManager(Database &database) : SessionManager(database)
    {}
    std::string new_session(User &user);
    void delete_session(Session &session);
    bool load_session(std::string const &sessionid, Session &session);
   
private:
    std::map<std::string, Session> _sessions;
};

#endif
