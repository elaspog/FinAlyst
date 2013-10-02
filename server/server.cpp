#include <iostream>
#include <fcgio.h>
#include <mysql/mysql.h>
#include <string>
#include <iostream>
#include <fstream>
#include <map>

using namespace std;

bool service_down = false;
std::string error_message;
std::map<std::string, std::string> config;

void parse_config(std::string const &filename)
{
    std::ifstream cf(filename);
    if (cf.is_open())
    {
        int lineno = 1;
        std::string line;
        while (std::getline(cf, line))
        {
            if (line.empty() || line[0] == '#') continue;
            size_t pos = line.find_first_of('=');
            if (pos == string::npos)
            {
                service_down = true;
                error_message = "Config file parse error on line " +
                    std::to_string(lineno) + ": " + line;
                return;
            }
            std::string key(line.substr(0, pos));
            std::string value(line.substr(pos+1, string::npos));
            config[key] = value;
            ++lineno;
        }
    } else
    {
        service_down = true;
        error_message = "Can't open config file: ";
        error_message += filename;
    }
}

int main(void) {
    FCGX_Request request;

    FCGX_Init();
    FCGX_InitRequest(&request, 0, 0);

    // Initialize application
    char const *config_file = getenv("CONFIG_FILE");
    if (config_file) parse_config(config_file);
    else {
        service_down = true;
        error_message = "CONFIG_FILE environment variable undefined";
    }

    // connect to database
    MYSQL *mysql = mysql_init(NULL);
    if (mysql == NULL) 
    {
        service_down = true;
        error_message = "Can't initialize mysql: ";
        error_message += mysql_error(mysql);
    }

    if (mysql_real_connect(mysql,
                config["database-server"].c_str(),
                config["database-user"].c_str(),
                config["database-pass"].c_str(),
                NULL, 0, NULL, 0) == NULL) 
    {
        service_down = true;
        error_message = "Can't connect to database: ";
        error_message += mysql_error(mysql);
    }

    // Accept requests
    while (FCGX_Accept_r(&request) == 0) {
        fcgi_streambuf isbuf(request.in);
        fcgi_streambuf osbuf(request.out);
        fcgi_streambuf esbuf(request.err);
        istream fcin(&isbuf);
        ostream fcout(&osbuf);
        ostream fcerr(&esbuf);

        fcout << "Content-type: text/html\r\n"
             << "\r\n"
             << "<html>\n"
             << "  <head>\n"
             << "    <title>Hello, World!</title>\n"
             << "  </head>\n"
             << "  <body>\n"
             << "    <h1>Hello, World!<h1>";

        if (service_down)
            fcout << "Error: " << error_message;
        else
            fcout << "Connected to mysql";

        fcout << "    <br>"
             << "  </body>\n"
             << "</html>\n";
    }

    // Close database connection
    mysql_close(mysql);
    return 0;
}
