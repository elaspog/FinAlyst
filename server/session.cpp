#include "session.hpp"

using namespace std;

static const size_t sessionid_rand_block_size = 64*sizeof(randgen_ret_type);

string SessionManager::generate_new_sessionid(User &user)
{
    string username = user.name();
    unsigned char data[username.size()+sizeof(time_t)+sessionid_rand_block_size];
    for (int i = 0; i < 64; ++i)
        *((randgen_ret_type*)data + i) = secure_rand();
    time((time_t*)(data+sessionid_rand_block_size));
    memcpy(data+sizeof(time_t)+sessionid_rand_block_size, username.c_str(), username.length());
    gcry_md_hd_t hd;
    gcry_error_t er = gcry_md_open(&hd, GCRY_MD_SHA512, 0);
    log_assert_equal(er, 0u);
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
            user = User::root();
            return true;
        }
        return false;
    }
    // Search for user
    user = User::find_by_name(database, username);
    return user.authenticate(password);
}

void webservice_login(Database &database, 
        std::unique_ptr<SessionManager> &sessionman,
        Request &request, std::ostream &fcout)
{
    html_content(fcout);
    fcout << "{\n";
    if (request.type() == RequestType::Post)
    {
        User user = User::find_by_name(database, request.post("username"));
        if (user.authenticate(request.post("password")))
        {
            LOG_MESSAGE_INFO("User %s logged in", user.name().c_str());
            fcout << "\t\"success\": true,\n";
            fcout << "\t\"status\": 200,\n";
            fcout << "\t\"data\": null,\n";
            fcout << "\t\"sessionid\": \"" << sessionman->new_session(user)
                << "\"" << endl;
        } else
        {
            LOG_MESSAGE_WARN("Login failed: authentication failed for user %s", request.post("username").c_str());
            fcout << "\t\"success\": false,\n";
            fcout << "\t\"status\": 401,\n";
            fcout << "\t\"data\": null\n";
        }
    } else
    {
        LOG_MESSAGE_WARN("Login failed: got GET request expected POST");
        fcout << "\t\"success\": false,\n";
        fcout << "\t\"status\": 405,\n";
        fcout << "\t\"data\": null\n";
    }
    fcout << "}";
}

void webservice_logout(std::unique_ptr<SessionManager> &sessionman,
        Session &session, std::ostream &fcout)
{
    html_content(fcout);
    fcout << "{\n";
    if (session.valid())
    {
        LOG_MESSAGE_INFO("User %s logged out", session.user().name().c_str());
        sessionman->delete_session(session);
        fcout << "\t\"success\": true,\n";
        fcout << "\t\"status\": 200,\n";
        fcout << "\t\"data\": null\n";
    } else
    {
        LOG_MESSAGE_WARN("Logout failed: invalid session");
        fcout << "\t\"success\": false,\n";
        fcout << "\t\"status\": 400,\n";
        fcout << "\t\"data\": null\n";
    }
    fcout << "}";
}

void login_page(OptsMap const &config, Database &database, 
        std::unique_ptr<SessionManager> &sessionman,
        Session &session, Request &request, std::ostream &fcout)
{
    bool login_failed = false;
    if (!session.valid())
    {
        std::string username, password;
        if (get_username_password(request, username, password))
        {
            User user;
            if (authenticate(database, config, username, password, user))
            {
                // Create a new session
                std::string sessionid = sessionman->new_session(user);
                LOG_MESSAGE_INFO("User %s logged in", user.name().c_str());
                // Set cookie and redirect to main page
                fcout << "Location: ?q=main\r\n";
                fcout << "Set-Cookie: sessionid=" << sessionid << "\r\n";
                fcout << "\r\n";
                return;
            } else
            {
                login_failed = true;
                LOG_MESSAGE_WARN("Login failed: authentication failed for user %s", username.c_str());
            }
        }
    } else
    {
        // Has valid session redirect to main page
        LOG_MESSAGE_INFO("User %s has vlaid session, redirecting to main page",
                session.user().name().c_str());
        fcout << "Location: ?q=main\r\n";
        fcout << "\r\n";
        return;
    }

    header(fcout, config, "Login");
    if (login_failed) fcout << "</span class=\"login_failed\">Login failed</span>";
    fcout << "<div id=\"login\">";
    fcout <<    "<h1 id=\"finalyst\">FinAlyst</h1>";
    fcout <<    "<h2 id=\"login\">Login</h2>";
    fcout <<    "<form name=\"login_form\" method=\"post\"><table>";
    fcout <<        "<tr><td>Username:</td><td><input name=\"username\"></td></tr>";
    fcout <<        "<tr><td>Password:</td><td><input type=\"password\" name=\"password\"></td></tr>";
    fcout <<        "</table><input type=\"submit\" value=\"Login\">";
    fcout <<    "</form>";
    fcout << "</div>";
    
    footer(fcout);
}

void logout_page(std::unique_ptr<SessionManager> &sessionman,
        Session &session, std::ostream &fcout)
{
    if (session.valid())
    {
        LOG_MESSAGE_INFO("User %s logged out", session.user().name().c_str());
        sessionman->delete_session(session);
    } else
    {
        LOG_MESSAGE_WARN("Logout failed: invalid session");
    }
    fcout << "Location: ?q=login\r\n";
    fcout << "Set-Cookie: sessionid=; expires=Thu, 01 Jan 1970 00:00:00 GMT";
    fcout << "\r\n\r\n";
}

