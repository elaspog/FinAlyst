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
        fcout <<     "<a href=\"?q=cpuinfo\">Cpu info</a>";
        fcout <<     "<a href=\"?q=meminfo\">Mem info</a>";
        fcout <<     "<a href=\"?q=hddstat\">HDD stat</a>";
        fcout <<     "<a href=\"?q=newuser\">New user</a>";
        fcout <<     "<a href=\"?q=webserviceapi\">Webservice API</a>";
        fcout <<     "<a href=\"?q=changelog\">Changelog</a>";
        fcout <<     "<a href=\"?q=logout\">Logout</a> ";
        fcout << "</div>";
    }

    void main_page(Database &database, Request &request, std::ostream &fcout)
    {
        std::vector<User> users;
        User::find_all(database, users);

        fcout << "<table><tr><th>Name</th><th>Create date</th><th></th></tr>" << std::endl;
        for (auto &user : users)
        {
            fcout << "<tr><td>";
            fcout << user.name() << "</td>";
            // TODO: create date
            fcout << "<td>" << "</td>";
            fcout << "<td><a href=\"finance?useredit&id=" << user.id() << "\">Edit</a></td>";
            fcout << "</tr>"<< std::endl;
        }
        fcout << "</table>";

        print_environment(fcout, request);
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
                    // TODO Error: different pass
                }
            } else
            {
                // TODO Error: missing username os password
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

    void cpuinfo(std::ostream &fcout)
    {
        std::ifstream f("/proc/cpuinfo");
        std::string line;
        while (std::getline(f, line)) fcout << line << "<br>" << std::endl;
    }

    void meminfo(std::ostream &fcout)
    {
        std::ifstream f("/proc/meminfo");
        std::string line;
        while (std::getline(f, line)) fcout << line << "<br>" << std::endl;
    }

    void hddstat(std::ostream &fcout)
    {
        struct statvfs stat;
        statvfs("/", &stat);
        
        fcout << "Total disk space: " << (stat.f_blocks * stat.f_frsize)/1024/1024 << "Mb<br>" << std::endl;
        fcout << "Free disk space: " << (stat.f_bavail * stat.f_bsize)/1024/1024 << "Mb<br>" << std::endl;
        fcout << "Total number of files: " << stat.f_files << "<br>" << std::endl;
    }

    void handle_request(OptsMap const &config, Database &database,
            Session &session, Request &request,
            std::ostream &fcout, std::istream &fcin)
    {
        // TODO
        (void)session;
        std::string query = request.query();
        switch (request.type())
        {
        case RequestType::Get:
            header(fcout, config, "Webservice API");
            menu(fcout);
            fcout << "<div class=\"content\">";

            if (query == "main")
                main_page(database, request, fcout);
            else if (query == "newuser")
                newuser_page(database, request, fcout, fcin);
            else if (query == "cpuinfo")
                cpuinfo(fcout);
            else if (query == "meminfo")
                meminfo(fcout);
            else if (query == "hddstat")
                hddstat(fcout);
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
    }

}

#endif
