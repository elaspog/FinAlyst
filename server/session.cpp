#include "session.hpp"

using namespace std;

static const size_t sessionid_rand_block_size = 64*sizeof(randgen_ret_type);

string SessionManager::generate_sessionid(User &user)
{
    string username = user.name();
    unsigned char data[username.size()+sizeof(time_t)+sessionid_rand_block_size];
    for (int i = 0; i < 64; ++i)
        *((randgen_ret_type*)data + i) = secure_rand();
    time((time_t*)(data+sessionid_rand_block_size));
    memcpy(data+sizeof(time_t)+sessionid_rand_block_size, username.c_str(), username.length());
    gcry_md_hd_t hd;
    gcry_error_t er = gcry_md_open(&hd, GCRY_MD_SHA512, 0);
    assert(er == 0);
    gcry_md_write(hd, (void*)&data, sizeof(data));
    unsigned char *digest = gcry_md_read(hd, GCRY_MD_SHA512);
    std::string sessionid;
    sessionid.resize(gcry_md_get_algo_dlen(GCRY_MD_SHA512)*2);
    int pos = 0;
    for (unsigned i = 0; i < gcry_md_get_algo_dlen(GCRY_MD_SHA512); ++i)
    {
        sprintf(&sessionid[pos], "%02X", digest[i]);
        pos += 2;
    }
    gcry_md_close(hd);
    return sessionid;
}

static bool get_username_password(Request &request,
        string &username, string &password)
{
    switch (request.type())
    {
    case RequestType::Get:
        // TODO
        //username = 
        break;
    case RequestType::Post:
        {
            username = request.post("username");
            password = request.post("password");
        }
        break;
    default:
        break;
    }
    return !username.empty() && !password.empty();
}

static bool authenticate(Database &database, OptsMap const &config,
        string const &username, string const &password,
        User &user)
{
    if (username == "root")
    {
        auto it = config.find("root-pass");
        if (it != config.end() && password == it->second)
        {
            user = User();
            user.name("root");
            user.id(0);
            return true;
        }
        return false;
    }
    // Search for user
    user = User::find_by_name(database, username);
    return user.authenticate(password);
}

void webservice_login(Database &database, 
        SessionManager &sessionman,
        Request &request, std::ostream &fcout)
{
    html_content(fcout);
    fcout << "{\n";
    if (request.type() == RequestType::Post)
    {
        User user = User::find_by_name(database, request.post("username"));
        if (user.authenticate(request.post("password")))
        {
            fcout << "\t\"sucess\": true,\n";
            fcout << "\t\"status\": 200,\n";
            fcout << "\t\"sessionid\": \"" << sessionman.new_session(user)
                << "\"" << endl;
        } else
        {
            fcout << "\t\"sucess\": false,\n";
            fcout << "\t\"status\": 401\n";
        }
    } else
    {
        fcout << "\t\"sucess\": false,\n";
        fcout << "\t\"status\": 405\n";
    }
    fcout << "}";
}

void webservice_logout(SessionManager &sessionman, Session *session,
        std::ostream &fcout)
{
    html_content(fcout);
    fcout << "{\n";
    if (session != NULL)
    {
        sessionman.delete_session(session->sessionid());
        fcout << "\t\"sucess\": true,\n";
        fcout << "\t\"status\": 200\n";
    } else
    {
        fcout << "\t\"sucess\": false,\n";
        fcout << "\t\"status\": 400\n";
    }
    fcout << "}";
}

void login_page(OptsMap const &config, Database &database, 
        SessionManager &sessionman, Session *session,
        Request &request, std::ostream &fcout)
{
    bool login_failed = false;
    if (session == NULL)
    {
        std::string username, password;
        if (get_username_password(request, username, password))
        {
            User user;
            if (authenticate(database, config, username, password, user))
            {
                // Create a new session
                std::string sessionid = sessionman.new_session(user);
                // Set cookie and redirect to main page
                fcout << "Location: ?q=main\r\n";
                fcout << "Set-Cookie: sessionid=" << sessionid << "\r\n";
                fcout << "\r\n";
                return;
            } else login_failed = true;
        }
    } else
    {
        // Has valid session redirect to main page
        fcout << "Location: ?q=main\r\n";
        fcout << "\r\n";
        return;
    }

    header(fcout, config, "Login");
    if (login_failed) fcout << "</span class=\"login_failed\">Login failed</span>";
    fcout << "<h1>Login</h1>";
    fcout << "<form name=\"login_form\" method=\"post\">";
    fcout << "Username: <input name=\"username\">";
    fcout << "Password: <input type=\"password\" name=\"password\">";
    fcout << "<input type=\"submit\" value=\"Login\">";
    fcout << "</form>";
    print_environment(fcout, request);
    
    footer(fcout);
}

void logout_page(SessionManager &sessionman, Session *session,
        std::ostream &fcout)
{
    if (session != NULL) sessionman.delete_session(session->sessionid());
    fcout << "Location: ?q=login\r\n";
    fcout << "Set-Cookie: sessionid=; expires=Thu, 01 Jan 1970 00:00:00 GMT";
    fcout << "\r\n\r\n";
}

