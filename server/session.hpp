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
    Session() : _valid(false) {}
    Session(User const &user, std::string const &sessionid) :
        _valid(true), _user(user), _sessionid(sessionid)
    {}

    User const& user() const { return _user; }

    std::string const& sessionid() const { return _sessionid; }

    bool valid()
    {
        return !_sessionid.empty();
    }

    void update_user()
    {
        log_assert(_user.valid());
        _user = User::find(_user.database(), _user.id());
    }

    bool valid() const { return _valid; }

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

private:
    bool _valid;
    User _user;
    std::string _sessionid;

    OptsMap _data;
};

class SessionManager
{
public:
    virtual std::string new_session(User &user) = 0;
    virtual void delete_session(Session &session) = 0;
    virtual bool load_session(std::string const &sessionid, Session &session) = 0;
protected:
    static std::string generate_new_sessionid(User &user);
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
