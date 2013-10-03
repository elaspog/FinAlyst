#include <iostream>
#include <fcgio.h>
#include <mysql/mysql.h>
#include <string>
#include <cstring>
#include <iostream>
#include <fstream>
#include <map>
#include <sys/statvfs.h>

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

void header(ostream &fcout)
{
    fcout << "Content-type: text/html\r\n"
         << "\r\n"
         << "<html>\n"
         << "  <head>\n"
         << "    <title>Hello, World!</title>\n"
         << "  </head>\n"
         << "  <body>\n";
}

void footer(ostream &fcout)
{
    fcout << "  </body>\n"
         << "</html>\n";
}

void login_page(FCGX_Request &request, ostream &fcout)
{
    header(fcout);

    fcout << "    <h1>Hello, World!</h1>";

    if (service_down)
        fcout << "Error: " << error_message << "<br>";
    else
        fcout << "Connected to mysql<br><br>";


    fcout << "<a href=\"?cpuinfo\">cpuinfo</a> ";
    fcout << "<a href=\"?meminfo\">meminfo</a> ";
    fcout << "<a href=\"?hddstat\">hddstat</a>";

    fcout << "<br><br>";

    // Print all environment variables
    fcout << "Environmental variables:<br>";
    char **env = request.envp;
    while (*(++env))
        fcout << *env << "<br>" << endl;
    footer(fcout);
}

void cpuinfo(ostream &fcout)
{
    header(fcout);
    ifstream f("/proc/cpuinfo");
    std::string line;
    while (std::getline(f, line)) fcout << line << "<br>" << endl;
    footer(fcout);
}

void meminfo(ostream &fcout)
{
    header(fcout);
    ifstream f("/proc/meminfo");
    std::string line;
    while (std::getline(f, line)) fcout << line << "<br>" << endl;
    footer(fcout);
}

void hddstat(ostream &fcout)
{
    header(fcout);
    struct statvfs stat;
    statvfs("/", &stat);
    fcout << "Total disk space: " << (stat.f_blocks * stat.f_frsize)/1024/1024 << "Mb<br>" << endl;
    fcout << "Free disk space: " << (stat.f_bavail * stat.f_bsize)/1024/1024 << "Mb<br>" << endl;
    fcout << "Total number of files: " << stat.f_files << "<br>" << endl;
    footer(fcout);
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

        const char *method = FCGX_GetParam("REQUEST_METHOD", request.envp);
        const char *query = FCGX_GetParam("QUERY_STRING", request.envp);
        if (strncmp(method, "GET", sizeof("GET")) == 0)
        {
            if (strncmp(query, "cpuinfo", sizeof("cpuinfo")) == 0)
                cpuinfo(fcout);
            else if (strncmp(query, "meminfo", sizeof("cpuinfo")) == 0)
                meminfo(fcout);
            else if (strncmp(query, "hddstat", sizeof("hddstat")) == 0)
                hddstat(fcout);
            else login_page(request, fcout);

        } else if (strncmp(method, "POST", sizeof("POST")) == 0)
        {
            // TODO
        } else {
            // TODO
        }
        
        FCGX_Finish_r(&request);
    }

    // Close database connection
    mysql_close(mysql);
    return 0;
}
