#ifndef ADMIN_HPP_INCLUDED
#define ADMIN_HPP_INCLUDED

#include <iostream>
#include <fstream>
#include <sys/statvfs.h>
#include "request.hpp"
#include "database.hpp"
#include "session.hpp"
#include "user.hpp"
#include "template_common.hpp"

namespace WebAdmin
{

    void menu(std::ostream &fcout)
    {
        fcout << "<div id=\"menu\">";
        fcout <<     "<a href=\"?q=main\">Main</a>";
        fcout <<     "<a href=\"?q=users\">Users</a>";
        fcout <<     "<a href=\"?q=webserviceapi\">Webservice API</a>";
        fcout <<     "<a href=\"?q=changelog\">Changelog</a>";
        fcout <<     "<a href=\"?q=logout\">Logout</a> ";
        fcout << "</div>";
    }

    void main_page(Database &database, Request &request, std::ostream &fcout)
    {
        (void)database; (void)request;

        struct statvfs stat;
        statvfs("/", &stat);
        fcout << "Total disk space: " << (stat.f_blocks * stat.f_frsize)/1024/1024 << "Mb<br>" << std::endl;
        fcout << "Free disk space: " << (stat.f_bavail * stat.f_bsize)/1024/1024 << "Mb<br>" << std::endl;
        fcout << "Total number of files: " << stat.f_files << "<br>" << std::endl;

        std::ifstream f("/proc/meminfo");
        std::string line;
        for (int i = 0; i < 2; ++i)
        {
            std::getline(f, line);
            fcout << line << "<br>" << std::endl;
        }

        std::ifstream cpuinfo("/proc/cpuinfo");
        while (std::getline(cpuinfo, line))
        {
            if (line.substr(0, strlen("Processor")) == "Processor" || line.substr(0, strlen("model name")) == "model name")
            {
                fcout << line << "<br>" << std::endl;
                break;
            }
        }
    }

    void users_page(Database &database,
            OptsMap const &config, std::ostream &fcout)
    {
        auto it = config.find("asset-dir");
        std::string assetpath;
        if (it != config.end()) assetpath = it->second;
        fcout << "<a href=\"?q=newuser\"><img alt=\"add user\" src=\""
            << assetpath << "/useradd.png\" width=\"32\"></a>";
        std::vector<User> users;
        User::find_all(database, users);

        fcout << "<table><tr><th>Name</th><th>Create date</th><th></th></tr>" << std::endl;
        for (auto &user : users)
        {
            char buffer[128];
            tm bt;
            time_t t = user.create();
            localtime_r(&t, &bt);
            strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M", &bt);
            fcout << "<tr><td>";
            fcout << user.name() << "</td>";
            fcout << "<td>" << buffer << "</td>";
            fcout << "<td><a href=\"?q=edituser&userid=" << user.id() << "\">"
                << "<img alt=\"edit\" width=\"32\" src=\"" << assetpath << "/edit.png\" ></a></td>";
            fcout << "</tr>"<< std::endl;
        }
        fcout << "</table>";
    }

    void newuser_page(Database &database, Request &request,
            std::ostream &fcout, std::istream &fcin)
    {
        if (request.type() == RequestType::Post)
        {
            std::map<std::string, std::string> data;
            parse_pairs(fcin, '&', data);
            auto name = data.find("name");
            auto pass = data.find("pass");
            auto pass_again = data.find("again");
            if (name != data.end() && pass != data.end())
            {
                if (pass_again == data.end() || pass_again->second == pass->second)
                {
                    User user(database, name->second, pass->second);
                    user.save();
                    fcout << "Location: ?q=main\r\n";
                    fcout << "\r\n\r\n";
                } else
                {
                    throw std::logic_error("Password different");
                }
            } else
            {
                throw std::logic_error("Missing username of password");
            }
        } else
        {
            fcout << "<form name=\"newuser_form\" method=\"post\">";
            fcout << "Username: <input name=\"name\">";
            fcout << "Password: <input name=\"pass\">";
            fcout << "Password again: <input name=\"pass_again\">";
            fcout << "<input type=\"submit\" value=\"Create\">";
            fcout << "</form>";
        }
    }

    void edituser_page(Database &database, Request &request,
            std::ostream &fcout, std::istream &fcin)
    {
        if (request.type() == RequestType::Post)
        {
            std::map<std::string, std::string> data;
            parse_pairs(fcin, '&', data);
            auto pass = data.find("pass");
            auto pass_again = data.find("again");
            std::string userid_str = request.post("userid");
            uint64_t userid;
            if (userid_str.empty() || !parse_unsigned(userid_str, userid))
                throw std::logic_error("Invalid or missing userid!");

            if (pass != data.end())
            {
                if (pass_again == data.end() || pass_again->second == pass->second)
                {
                    User user = User::find(database, userid);
                    if (!user.valid())
                    {
                        LOG_MESSAGE_DEBUG("No such user");
                        throw std::logic_error("No such user!");
                    }
                    //user.name(name->second);
                    user.reset_password(pass->second);
                    user.save();
                    fcout << "Location: ?q=user\r\n";
                    fcout << "\r\n\r\n";
                } else
                {
                    LOG_MESSAGE_DEBUG("Password different");
                    throw std::logic_error("Password different");
                }
            } else
            {
                LOG_MESSAGE_DEBUG("Missing password");
                throw std::logic_error("Missing password");
            }
        } else
        {
            fcout << "<form name=\"edituser_form\" method=\"post\">";
            fcout << "<input type=\"hidden\" name=\"userid\" value=\"" << request.get("userid") << "\">";
            //fcout << "Username: <input name=\"name\">";
            fcout << "Password: <input name=\"pass\">";
            fcout << "Password again: <input name=\"pass_again\">";
            fcout << "<input type=\"submit\" value=\"Edit\">";
            fcout << "</form>";
        }
    }

    void handle_request(OptsMap const &config, Database &database,
            Session &session, Request &request,
            std::ostream &fcout, std::istream &fcin)
    {
        (void)session;
        std::string query = request.query();
        try {
            switch (request.type())
            {
            case RequestType::Get:
                header(fcout, config, "Webservice API");
                menu(fcout);
                fcout << "<div class=\"content\">";

                if (query == "main")
                    main_page(database, request, fcout);
                else if (query == "users")
                    users_page(database, config, fcout);
                else if (query == "newuser")
                    newuser_page(database, request, fcout, fcin);
                else if (query == "edituser")
                    edituser_page(database, request, fcout, fcin);
                else if (query == "webserviceapi")
                    static_page("webserviceapi.html", config, fcout);
                else if (query == "changelog")
                    static_page("changelog.html", config, fcout);
                else
                {
                    // TODO: handle 404 better
                    fcout << "Error: 404";
                }

                fcout << "</div>";
                footer(fcout);
                break;
            case RequestType::Post:
                if (query == "newuser")
                    newuser_page(database, request, fcout, fcin);
                else
                {
                    // TODO: 404
                }
                break;
            default:
                // TODO
                break;
            }
        } catch (std::exception const &error)
        {
            LOG_MESSAGE_WARN("Webadmin unknown error: %s", error.what());
        } catch (...)
        {
            LOG_MESSAGE_WARN("Webadmin mysterious unknown error. This should never happen!");
        }
    }

}

#endif
