#include "session_mem.hpp"

using std::string;

string MemorySessionManager::new_session(User &user)
{
    string sessionid = generate_new_sessionid(user);
    _sessions[sessionid] = Session(user, sessionid);
    return sessionid;
}

void MemorySessionManager::delete_session(Session &session)
{
    _sessions.erase(session.sessionid());
}

bool MemorySessionManager::load_session(string const &sessionid, Session &session)
{
    auto it = _sessions.find(sessionid);
    if (it != _sessions.end())
    {
        session = it->second;
        session.update_user();
        return true;
    }
    return false;
}
