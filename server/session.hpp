#ifndef SESSION_HPP_INCLUDED
#define SESSION_HPP_INCLUDED

#include <limits>
#include <cstdint>
#include <string>

#include "user.hpp"
#include "request.hpp"
#include "template_common.hpp"

struct Session
{
public:
    Session() {}
    Session(User const &user, std::string const &sessionid) :
        _user(user), _sessionid(sessionid)
    {}

    User const& user() const { return _user; }

    std::string const& sessionid() const { return _sessionid; }

    bool valid()
    {
        return !_sessionid.empty();
    }

    void update_user()
    {
        _user = User::find(_user.database(), _user.id());
    }

private:
    User _user;
    std::string _sessionid;
};

class SessionManager
{
public:
    std::string new_session(User &user)
    {
        std::string sessionid = generate_sessionid(user);
        _sessions[sessionid] = Session(user, sessionid);
        return sessionid;
    }

    void delete_session(std::string const &sessionid)
    {
        _sessions.erase(sessionid);
    }

    Session* session(std::string const &sessionid)
    {
        auto it = _sessions.find(sessionid);
        if (it != _sessions.end()) return &(it->second);
        return NULL;
    }

private:

    static std::string generate_sessionid(User &user);
    
    std::map<std::string, Session> _sessions;
};

void webservice_login(Database &database, SessionManager &sessionman,
        Request &request, std::ostream &fcout);

void webservice_logout(SessionManager &sessionman, Session *session,
        std::ostream &fcout);

void login_page(OptsMap const &config, Database &database, 
        SessionManager &sessionman, Session *session,
        Request &request,
        std::ostream &fcout);

void logout_page(SessionManager &sessionman, Session *session,
        std::ostream &fcout);

#endif
