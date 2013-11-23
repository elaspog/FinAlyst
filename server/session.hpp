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

    void set(User const &user, std::string const &sessionid)
    {
        log_assert(!valid()); // TODO: report securoty problem
        if (user.valid())
        {
            _sessionid = sessionid;
            _user = user;
        }
    }

    User const& user() const { return _user; }
    User& user() { return _user; }

    std::string const& sessionid() const { return _sessionid; }

    bool valid() const
    {
        return !_sessionid.empty();
    }

    void update_user()
    {
        log_assert(_user.valid());
        _user = User::find(_user.database(), _user.id());
    }

    std::string get(std::string const &key) const
    {
        auto it = _data.find(key);
        if (it == _data.end()) return "";
        return it->second;
    }

    void set(std::string const &key, std::string const &value)
    {
        _data[key] = value;
    }

    OptsMap const& data() const { return _data; }
    OptsMap& data() { return _data; }

private:
    User _user;
    std::string _sessionid;

    OptsMap _data;
};

class SessionManager
{
public:
    SessionManager(Database &database) :
        _database(database), _session_timeout(60*60)
    {}

    unsigned session_timeout() { return _session_timeout; }
    Database& database() { return _database; }

    virtual std::string new_session(User &user) = 0;
    virtual void update_session(Session &session) { (void)session; };
    virtual void delete_session(Session &session) = 0;
    virtual bool load_session(std::string const &sessionid, Session &session) = 0;
protected:
    static std::string generate_new_sessionid(User &user);
private:
    Database &_database;
    unsigned _session_timeout;
};

void webservice_login(Database &database,
        std::unique_ptr<SessionManager> &sessionman,
        Request &request, std::ostream &fcout);

void webservice_logout(std::unique_ptr<SessionManager> &sessionman,
        Session &session, std::ostream &fcout);

void login_page(OptsMap const &config, Database &database, 
        std::unique_ptr<SessionManager> &sessionman,
        Session &session, Request &request, std::ostream &fcout);

void logout_page(std::unique_ptr<SessionManager> &sessionman,
        Session &session, std::ostream &fcout);

#endif
