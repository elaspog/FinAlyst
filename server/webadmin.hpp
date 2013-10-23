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
        fcout << "<a href=\"?q=cpuinfo\">cpuinfo</a> ";
        fcout << "<a href=\"?q=meminfo\">meminfo</a> ";
        fcout << "<a href=\"?q=hddstat\">hddstat</a> ";
        fcout << "<a href=\"?q=newuser\">newuser</a> ";
        fcout << "<a href=\"?q=logout\">logout</a> ";
    }

    void main_page(OptsMap const &config, Database &database, Request &request, std::ostream &fcout)
    {
        header(fcout, config, "Administration");
        menu(fcout);

        fcout << "<br><br>";
        fcout << "<h1>Administration</h1>";

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

        footer(fcout);
    }

    void newuser_page(OptsMap const &config,
            Database &database, Request &request,
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
            header(fcout, config, "New user");
            menu(fcout);

            fcout << "<form name=\"newuser_form\" method=\"post\">";
            fcout << "Username: <input name=\"name\">";
            fcout << "Password: <input name=\"pass\">";
            fcout << "Password again: <input name=\"pass_again\">";
            fcout << "<input type=\"submit\" value=\"Create\">";
            fcout << "</form>";

            footer(fcout);
        }
    }

    void cpuinfo(OptsMap const &config, std::ostream &fcout)
    {
        header(fcout, config, "CPU info");
        menu(fcout);

        std::ifstream f("/proc/cpuinfo");
        std::string line;
        while (std::getline(f, line)) fcout << line << "<br>" << std::endl;
        footer(fcout);
    }

    void meminfo(OptsMap const &config, std::ostream &fcout)
    {
        header(fcout, config, "MEM info");
        menu(fcout);

        std::ifstream f("/proc/meminfo");
        std::string line;
        while (std::getline(f, line)) fcout << line << "<br>" << std::endl;
        footer(fcout);
    }

    void hddstat(OptsMap const &config, std::ostream &fcout)
    {
        header(fcout, config, "HDD stat");
        menu(fcout);

        struct statvfs stat;
        statvfs("/", &stat);
        fcout << "Total disk space: " << (stat.f_blocks * stat.f_frsize)/1024/1024 << "Mb<br>" << std::endl;
        fcout << "Free disk space: " << (stat.f_bavail * stat.f_bsize)/1024/1024 << "Mb<br>" << std::endl;
        fcout << "Total number of files: " << stat.f_files << "<br>" << std::endl;
        footer(fcout);
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
            if (query == "main")
                main_page(config, database, request, fcout);
            else if (query == "newuser")
                newuser_page(config, database, request, fcout, fcin);
            else if (query == "cpuinfo")
                cpuinfo(config, fcout);
            else if (query == "meminfo")
                meminfo(config, fcout);
            else if (query == "hddstat")
                hddstat(config, fcout);
            else
            {
                // TODO: 404
            }
            break;
        case RequestType::Post:
            if (query == "newuser")
                newuser_page(config, database, request, fcout, fcin);
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
