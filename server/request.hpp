#ifndef REQUEST_HPP_INCLUDED
#define REQUEST_HPP_INCLUDED

#include <functional>
#include <string>
#include <map>
#include <fcgio.h>
#include <stdexcept>
#include <sstream>
#include <cstring>

typedef std::map<std::string, std::string> OptsMap;
typedef std::function<void (std::string const&, std::string const&)> OptsCb;

enum class RequestType { Unknown, Get, Post };

void parse_pairs(std::istream &urldata, char pair_delimiter, std::map<std::string, std::string> &pairs);

class Request
{
public:

    Request(FCGX_Request &request) : //request(request),
        _type(RequestType::Unknown),
        isbuf(request.in), osbuf(request.out), esbuf(request.err),
        fcin(&isbuf), fcout(&osbuf), fcerr(&esbuf), _postset(false)
    {
        // Parse environment variables
        char **e = request.envp;
        while (*(++e))
        {
            char *eq = strchr(*e, '=');
            if (eq == NULL) throw std::logic_error(
                std::string("Malformed environment variable: ")+*e);
            _env[std::string(*e, eq-(*e))] = std::string(eq+1);
        }
        // Parse cookies
        std::stringstream cookiestream(env("HTTP_COOKIE"));
        parse_pairs(cookiestream, ';', _cookies);
        // Determine request type
        std::string method = env("REQUEST_METHOD");
        if (method == "GET")
            _type = RequestType::Get;
        if (method == "POST")
            _type = RequestType::Post;
        // Parse get arguments
        std::stringstream querystream(env("QUERY_STRING"));
        parse_pairs(querystream, '&', _get);
        auto it = _get.find("q");
        if (it != _get.end()) _query = it->second;
    }

    std::string env(std::string const &key) const
    {
        auto it = _env.find(key);
        if (_env.end() != it) return it->second;
        return "";
    }

    std::string cookie(std::string const &key) const
    {
        auto it = _cookies.find(key);
        if (_cookies.end() != it) return it->second;
        return "";
    }

    bool post(std::string const &key, std::string &value)
    {
        need_post();
        auto it = _post.find(key);
        if (_post.end() != it)
        {
            value = it->second;
            return true;
        }
        return false;
    }

    std::string post(std::string const &key)
    {
        std::string value;
        post(key, value);
        return value;
    }

    bool get(std::string const &key, std::string &value) const
    {
        auto it = _get.find(key);
        if (_get.end() != it)
        {
            value = it->second;
            return true;
        }
        value = "";
        return false;
    }

    std::string get(std::string const &key) const
    {
        std::string value;
        get(key, value);
        return value;
    }

    void each_env(OptsCb cb) const
    {
        for (auto &env : _env)
            cb(env.first, env.second);
    }

    void each_cookie(OptsCb cb) const
    {
        for (auto &cookie : _cookies)
            cb(cookie.first, cookie.second);
    }

    RequestType type() const { return _type; }
    std::string query() const { return _query; }

private:

    void need_post()
    {
        if (_postset) return;
        _postset = true;
        if (env("CONTENT_TYPE") == "application/x-www-form-urlencoded")
        {
            if (_type == RequestType::Post)
            {
                try {
                    parse_pairs(fcin, '&', _post);
                } catch (...)
                {
                    // TODO
                }
            }
        }
    }
 
    //FCGX_Request &request;
    RequestType _type;
    fcgi_streambuf isbuf;
    fcgi_streambuf osbuf;
    fcgi_streambuf esbuf;
    std::istream fcin;
    std::ostream fcout;
    std::ostream fcerr;
    std::string _query;
    OptsMap _env;
    OptsMap _cookies;
    mutable OptsMap _post;
    mutable bool _postset;
    OptsMap _get;
};

#endif
