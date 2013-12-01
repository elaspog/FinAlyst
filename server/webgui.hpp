#ifndef WEBGUI_HPP_INCLUDED
#define WEBGUI_HPP_INCLUDED

#include <iostream>
#include <algorithm>
#include <time.h>

#include "session.hpp"
#include "database.hpp"
#include "category.hpp"
#include "planitem.hpp"
#include "item.hpp"
#include "template_common.hpp"
#include "utils.hpp"
#include "bulogic.hpp"

namespace WebGUI
{

    void menu(std::ostream &fcout)
    {
        fcout << "<div id=\"menu\">";
        fcout << "<a href=\"?q=category\">Category</a>";
        fcout << "<a href=\"?q=item\">Items</a>";
        fcout << "<a href=\"?q=plan\">Plan</a>";
        fcout << "<a href=\"?q=daily_overview\">Overview</a>";
        fcout << "<a href=\"?q=diagram\">Diagrams</a>";
        fcout << "<a href=\"?q=logout\">Logout</a> ";
        fcout << "</div>";
    }

    void main_page(std::ostream &fcout)
    {
        fcout << "<h2>Üdvözül a FinAlyst</h2>";
        footer(fcout);
    }

    void item_page(Database &database, Session &session,
            Request &request, OptsMap const &config, std::ostream &fcout)
    {
        auto it = config.find("asset-dir");
        std::string assetpath;
        if (it != config.end()) assetpath = it->second;

        std::string pagestr = request.get("page");
        uint64_t page;
        if (!parse_unsigned(pagestr, page)) page = 0;
        
        fcout << "<div class=\"date\"><span class=\"arrow\">";
        fcout << "<a href=\"?q=item&page="
            << page + 1
            << "\"><img alt=\"left\" width=\"32\" src=\""
            << assetpath << "/arrow-left.png\"></img></a>";
        fcout << "</span>";

        fcout << "<span style=\"height: 32px\">Item</span>";

        if (page > 0)
        {
            fcout << "<span class=\"arrow\">";
            fcout << "<a href=\"?q=item&page="
                << page - 1
                << "\"><img alt=\"right\" width=\"32\" src=\""
                << assetpath << "/arrow-right.png\"></img></a>";
            fcout << "</span>";

            fcout << "<span class=\"arrow\">";
            fcout << "<a href=\"?q=item&page=0"
                << "\"><img alt=\"right\" width=\"32\" src=\""
                << assetpath << "/arrow-right-double.png\"></img></a>";
        }
        fcout << "</span></div>";

        fcout << "<a href=\"?q=item_add\"><img alt=\"add item\" src=\""
            << assetpath << "/add.png\" width=\"32\">Add item</a>";
        std::vector<std::pair<Category, Item>> data;
        Item::find_all_with_category(database, session.user(), data, 30, page);
        fcout << "<table><tr><th>Dátum</th><th>Category</th><th>Amount</th><th>Description</th><th></th></tr>";
        for (auto &item : data)
        {
            char buffer[128];
            tm bt;
            time_t t = item.second.create();
            localtime_r(&t, &bt);
            strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M", &bt);
            fcout << "<tr>";
            fcout << "<td>" << buffer << "</td>";
            fcout << "<td>" << item.first.name() << "</td>";
            fcout << "<td>" << item.second.amount() << "</td>";
            fcout << "<td>" << item.second.description() << "</td>";
            fcout <<   "<td>";
            fcout <<     "<a href=\"?q=item_edit&itemid=" << item.second.id() << "\">Edit</a>";
            fcout <<     "<a rel=\"nofollow\" href=\"?q=item_destroy_conform&itemid=" << item.second.id() << "\">Delete</a>";
            fcout <<   "</td>";
            fcout << "</tr>";
        }
        fcout << "</table>";
    }

    void category_page(Database &database, Session &session,
            Request &request, std::ostream &fcout)
    {
        // TODO: handle limit
        (void)request;
        fcout << "<a href=\"?q=category_add\">Add category</a>";
        fcout << "<h2>Categories:</h2>";
        std::vector<Category> categories;
        Category::find_all(database, session.user(), categories);
        fcout << "<table><tr><th>Név</th><th>Megjegyzés</th><th></th></tr>";
        for (auto &category : categories)
        {
            fcout << "<tr>";
            fcout << "<td>" << category.name() << "</td>";
            fcout << "<td>" << category.description() << "</td>";
            fcout <<   "<td>";
            fcout <<     "<a href=\"?q=category_edit&categoryid=" << category.id() << "\">Edit</a>";
            fcout <<     "<a rel=\"nofollow\" href=\"?q=category_destroy_conform&categoryid=" << category.id() << "\">Delete</a>";
            fcout <<   "</td>";
            fcout << "</tr>";
        }
        fcout << "</table>";
    }

    void item_add(Database &database, Session &session,
            Request &request, std::ostream &fcout)
    {
        if (request.type() == RequestType::Post)
        {
            BusinessLogic::item_add(database, session, request);
            fcout << "Location: ?q=item\r\n";
            fcout << "\r\n\r\n";
            return;
        }

        fcout << "<br />";
        fcout << "<form name=\"item_add_form\"  method=\"post\" action=\"?q=item_add\">";
        fcout << "<select name=\"categoryid\">";
        std::vector<Category> categories;
        Category::find_all(database, session.user(), categories);
        for (auto &category : categories)
        {
            fcout << "<option value=\"" << category.id() <<  "\">"
                << category.name() << "</option>";
        }
        fcout << "</select>";
        fcout << "Amount: <input name=\"amount\">";
        fcout << "Description: <textarea rows=\"3\" name=\"description\"></textarea>";
        fcout << "<input type=\"submit\" value=\"Add item\">";
        fcout << "</form>";
    }

    void plan_page(Database &database, Session &session,
            Request &request, std::ostream &fcout)
    {
        // TODO: handle limit
        (void)request;
        fcout << "<a href=\"?q=planitem_add\">Add plan item</a>";
        fcout << "<h2>Plan</h2>";
        std::vector<std::pair<Category, PlanItem>> data;
        PlanItem::find_all_with_category(database, session.user(), data);
        fcout << "<table><tr><th>Dátum</th><th>Category</th><th>Amount</th><th>Description</th><th></th></tr>";
        for (auto &item : data)
        {
            char buffer[128];
            tm bt;
            time_t t = item.second.create();
            localtime_r(&t, &bt);
            strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M", &bt);
            fcout << "<tr>";
            fcout << "<td>" << buffer << "</td>";
            fcout << "<td>" << item.first.name() << "</td>";
            fcout << "<td>" << item.second.amount() << "</td>";
            fcout << "<td>" << item.second.description() << "</td>";
            fcout <<   "<td>";
            fcout <<     "<a href=\"?q=planitem_edit&planitemid=" << item.second.id() << "\">Edit</a>";
            fcout <<     "<a rel=\"nofollow\" href=\"?q=planitem_destroy_conform&planitemid=" << item.second.id() << "\">Delete</a>";
            fcout <<   "</td>";
            fcout << "</tr>";
        }
        fcout << "</table>";
    }

    void planitem_add(Database &database, Session &session,
            Request &request, std::ostream &fcout)
    {
        if (request.type() == RequestType::Post)
        {
            BusinessLogic::planitem_add(database, session, request);
            fcout << "Location: ?q=plan\r\n";
            fcout << "\r\n\r\n";
            return;
        }

        fcout << "<form name=\"planitem_add_form\"  method=\"post\" action=\"?q=planitem_add\">";
        fcout << "<select name=\"categoryid\">";
        std::vector<Category> categories;
        Category::find_all(database, session.user(), categories);
        for (auto &category : categories)
        {
            fcout << "<option value=\"" << category.id() <<  "\">"
                << category.name() << "</option>";
        }
        fcout << "</select>";
        fcout << "Amount: <input name=\"amount\">";
        fcout << "Description: <textarea rows=\"3\" name=\"description\"></textarea>";
        fcout << "<input type=\"submit\" value=\"Add plan item\">";
        fcout << "</form>";
    }

    void category_add(Database &database, Session &session,
            Request &request, std::ostream &fcout)
    {
        if (request.type() == RequestType::Post)
        {
            BusinessLogic::category_add(database, session, request);
            fcout << "Location: ?q=category\r\n";
            fcout << "\r\n\r\n";
            return;
        }

        fcout << "<br />";
        fcout << "<form name=\"category_add_form\" method=\"post\" action=\"?q=category_add\">";
        fcout << "Name: <input name=\"name\">";
        fcout << "Description: <textarea rows=\"3\" name=\"description\"></textarea>";
        fcout << "<input type=\"submit\" value=\"Kategória hozzáadása\">";
        fcout << "</form>";
    }

    void category_edit(Database &database, Session &session,
            Request &request, std::ostream &fcout)
    {
        if (request.type() == RequestType::Post)
        {
            BusinessLogic::category_edit(database, session, request);
            fcout << "Location: ?q=category\r\n";
            fcout << "\r\n\r\n";
            return;
        }
        uint64_t categoryid;
        if (!parse_unsigned(request.get("categoryid"), categoryid))
        {
            // TODO
            throw std::logic_error("categoryid not a number");
        }
        Category category = Category::find(database, categoryid);
        if (!category.valid())
        {
            // TODO
            throw std::logic_error("category does not exists");
        }
        fcout << "<br />";
        fcout << "<form name=\"category_edit_form\" method=\"post\" action=\"?q=category_edit\">";
        fcout << "<input type=\"hidden\" name=\"categoryid\" value=\""
            << category.id() <<  "\">";
        fcout << "Name: <input name=\"name\" value=\"";
            htmlspecialchars(category.name(), fcout);
            fcout << "\">";
        fcout << "Description: <textarea rows=\"3\" name=\"description\">";
            htmlspecialchars(category.description(), fcout);
            fcout << "</textarea>";
        fcout << "<input type=\"submit\" value=\"Save\">";
        fcout << "</form>";
    }

    void item_edit(Database &database, Session &session,
            Request &request, std::ostream &fcout)
    {
        if (request.type() == RequestType::Post)
        {
            BusinessLogic::item_edit(database, session, request);
            fcout << "Location: ?q=item\r\n";
            fcout << "\r\n\r\n";
            return;
        }
        uint64_t itemid;
        if (!parse_unsigned(request.get("itemid"), itemid))
        {
            // TODO
            throw std::logic_error("itemid not a number");
        }
        Item item = Item::find(database, itemid);
        if (!item.valid())
        {
            // TODO
            throw std::logic_error("item does not exists");
        }
        fcout << "<br />";
        fcout << "<form name=\"item_edit_form\" method=\"post\" action=\"?q=item_edit\">";
        fcout << "<input type=\"hidden\" name=\"itemid\" value=\""
            << item.id() <<  "\">";
        fcout << "Amount: <input name=\"amount\" value=\""
            << item.amount() << "\">";
        fcout << "Description: <textarea rows=\"3\" name=\"description\">";
            htmlspecialchars(item.description(), fcout);
            fcout << "</textarea>";
        fcout << "<input type=\"submit\" value=\"Save\">";
        fcout << "</form>";
    }

    void planitem_edit(Database &database, Session &session,
            Request &request, std::ostream &fcout)
    {
        if (request.type() == RequestType::Post)
        {
            BusinessLogic::planitem_edit(database, session, request);
            fcout << "Location: ?q=plan\r\n";
            fcout << "\r\n\r\n";
            return;
        }
        uint64_t planitemid;
        if (!parse_unsigned(request.get("planitemid"), planitemid))
        {
            // TODO
            throw std::logic_error("planitemid not a number");
        }
        PlanItem planitem = PlanItem::find(database, planitemid);
        if (!planitem.valid())
        {
            // TODO
            throw std::logic_error("planitem does not exists");
        }
        fcout << "<br />";
        fcout << "<form name=\"planitem_edit_form\" method=\"post\" action=\"?q=planitem_edit\">";
        fcout << "<input type=\"hidden\" name=\"planitemid\" value=\""
            << planitem.id() <<  "\">";
        fcout << "Amount: <input name=\"amount\" value=\""
            << planitem.amount() << "\">";
        fcout << "Description: <textarea rows=\"3\" name=\"description\">";
            htmlspecialchars(planitem.description(), fcout);
            fcout << "</textarea>";
        fcout << "<input type=\"submit\" value=\"Save\">";
        fcout << "</form>";
    }

    void category_destroy_conform(Request &request, std::ostream &fcout)
    {
        fcout << "<form method=\"POST\" action=\"?q=category_destroy\">";
            fcout << "Are you sure you want to delete?";
            fcout << "<input type=\"hidden\" name=\"categoryid\" value=\""
                << request.get("categoryid") << "\">";
            fcout << "<a href=\"?q=category\">Back</a>";
            fcout << "<input type=\"submit\" value=\"Delete\">";
        fcout << "</form>";
    }

    void item_destroy_conform(Request &request, std::ostream &fcout)
    {
        fcout << "<form method=\"POST\" action=\"?q=item_destroy\">";
            fcout << "Are you sure you want to delete?";
            fcout << "<input type=\"hidden\" name=\"itemid\" value=\""
                << request.get("itemid") << "\">";
            fcout << "<a href=\"?q=item\">Back</a>";
            fcout << "<input type=\"submit\" value=\"Delete\">";
        fcout << "</form>";
    }

    void planitem_destroy_conform(Request &request, std::ostream &fcout)
    {
        fcout << "<form method=\"POST\" action=\"?q=planitem_destroy\">";
            fcout << "Are you sure you want to delete?";
            fcout << "<input type=\"hidden\" name=\"planitemid\" value=\""
                << request.get("planitemid") << "\">";
            fcout << "<a href=\"?q=plan\">Back</a>";
            fcout << "<input type=\"submit\" value=\"Delete\">";
        fcout << "</form>";
    }

    void category_destroy(Database &database, Session &session,
            Request &request, std::ostream &fcout)
    {
        if (request.type() == RequestType::Post)
        {
            BusinessLogic::category_destroy(database, session, request);
            fcout << "Location: ?q=category\r\n";
            fcout << "\r\n\r\n";
        } else {
            javascriptredirect(fcout, "?q=category");
        }
    }

    void item_destroy(Database &database, Session &session,
            Request &request, std::ostream &fcout)
    {
        if (request.type() == RequestType::Post)
        {
            BusinessLogic::item_destroy(database, session, request);
            fcout << "Location: ?q=item\r\n";
            fcout << "\r\n\r\n";
        } else {
            javascriptredirect(fcout, "?q=item");
        }
    }

    void planitem_destroy(Database &database, Session &session,
            Request &request, std::ostream &fcout)
    {
        if (request.type() == RequestType::Post)
        {
            BusinessLogic::planitem_destroy(database, session, request);
            fcout << "Location: ?q=plan\r\n";
            fcout << "\r\n\r\n";
        } else {
            javascriptredirect(fcout, "?q=plan");
        }
    }

    void balance_stats(Database &database, Session &session,
            Request &request, std::ostream &fcout)
    {
        std::vector<Category> categories;
        Category::find_all(database, session.user(), categories);

        // Get arguments
        std::string categoryidstr = request.get("categoryid");
        std::string granulation = request.get("granulation");

        // Parse arguments
        uint64_t categoryid;
        if (!parse_unsigned(categoryidstr, categoryid))
            categoryid = categories[0].id();

        Category::StatGranulation gran = Category::StatGranulation::weekly;
        if (granulation == "week")
            gran = Category::StatGranulation::weekly;
        else if (granulation == "month")
            gran = Category::StatGranulation::monthly;
        else if (granulation == "year")
            gran = Category::StatGranulation::yearly;

        fcout << "<h2>Balance</h2>";

        fcout << "<form name=\"balance_stats\"  method=\"get\" action=\"\" style=\"margin-bottom: 20px\">";
        fcout << "<input type=\"hidden\" name=\"q\" value=\"diagram\">";
        fcout << "<select name=\"categoryid\">";
        for (auto &category : categories)
        {
            if (category.id() == categoryid)
                fcout << "<option value=\"" << category.id() <<  "\" selected=\"selected\">"
                    << category.name() << "</option>";
            else
                fcout << "<option value=\"" << category.id() <<  "\">"
                    << category.name() << "</option>";
        }
        fcout << "</select>";
        fcout << "<select name=\"granulation\">";
        if (gran == Category::StatGranulation::weekly)
            fcout << "<option value=\"week\" selected=\"selected\">weekly</option>";
        else
            fcout << "<option value=\"week\">weekly</option>";

        if (gran == Category::StatGranulation::monthly)
            fcout << "<option value=\"month\" selected=\"selected\">monthly</option>";
        else
            fcout << "<option value=\"month\">monthly</option>";

        if (gran == Category::StatGranulation::yearly)
            fcout << "<option value=\"year\" selected=\"selected\">yearly</option>";
        else
            fcout << "<option value=\"year\">yearly</option>";
        fcout << "</select>";
        fcout << "<input type=\"submit\" value=\"Query\">";
        fcout << "</form>";
        
        std::vector<Category::BalanceData> data;
        auto category = std::find_if(categories.begin(), categories.end(),
                [categoryid](Category const &c) { return c.id() == categoryid; });
        if (category == categories.end())
        {
            fcout << "Category does not exists!";
            return;
        }
        LOG_MESSAGE_DEBUG("Mókuska");
        time_t t;
        tm bt;
        time(&t);
        localtime_r(&t, &bt);
        unsigned year = bt.tm_year + 1900;
        LOG_MESSAGE_DEBUG("year: %u\n", year);
        category->balance_stats(data, gran, year);
        if (gran == Category::StatGranulation::yearly)
        {
            // calculate previous 5 years
            for (int i = 1; i <= 5; ++i) category->balance_stats(data, gran, year - i);
        }
        fcout << "<div class=\"chart-wrapper\"><div class=\"chart\" style=\"height:500px;width:" << data.size()*70<< "px\">";
        unsigned left = 0;
        int last_month = -1;
        auto max_expense = std::max_element(data.begin(), data.end(), 
                [](Category::BalanceData const &a, Category::BalanceData const &b) {
                    return a.expensesum < b.expensesum;
                });
        auto max_plan = std::max_element(data.begin(), data.end(), 
                [](Category::BalanceData const &a, Category::BalanceData const &b) {
                    return a.plannedsum < b.plannedsum;
                });
        unsigned max = std::max(max_expense->expensesum, max_plan->plannedsum);
        for (auto &b : data)
        {
            int height = (double)b.expensesum/max*420;
            fcout << "<div class=\"expense\" style=\"z-index:1;left:" << left
                << "px;bottom:80px;height:" << height << "px;width:30px;\">";
            if (height > 50) fcout
                << "<div style=\"height:30px;line-height:30px;transform-origin: 0% 0%; bottom: -25px; transform: rotate(-90deg);\">"
                << b.expensesum << "</div>";
            fcout << "</div>";
            height = (double)b.plannedsum/max*420;
            fcout << "<div class=\"plan\" style=\"left:" << left+25
                << "px;bottom:80px;height:" << height << "px;width:30px;\">";
            if (height > 50) fcout
                << "<div style=\"height:30px;line-height:30px;transform-origin: 0% 0%; bottom: -25px; transform: rotate(-90deg);\">"
                << b.plannedsum << "</div>";
            fcout << "</div>";
            left += 70;
            char buffer[128];
            localtime_r(&t, &bt);
            switch (gran)
            {
            case Category::StatGranulation::weekly:
                // Calculate month
                bt.tm_mon = 0;
                bt.tm_mday = b.interval * 7;
                mktime(&bt);
                if (bt.tm_mon > last_month)
                {
                    strftime(buffer, sizeof(buffer), "%B", &bt);
                    last_month = bt.tm_mon;
                    fcout << "<div class=\"label\" style=\"left:" << left+20
                    << "px;bottom:0px;height:50px;line-height:50px\">" << buffer << "</div>";
                }
            break;
            case Category::StatGranulation::monthly:
                bt.tm_mon = b.interval - 1;
                mktime(&bt);
                strftime(buffer, sizeof(buffer), "%B", &bt);
                fcout << "<div class=\"label\" style=\"left:"
                    << (int)left - 90
                    << "px;bottom:-50px;height:50px;line-height:50px;transform-origin: 0% 0%; transform:rotate(-60deg);font-size: 18px\">"
                    << buffer << "</div>";
            break;
            case Category::StatGranulation::yearly:
                fcout << "<div class=\"label\" style=\"left:"
                    << (int)left - 90
                    << "px;bottom:-50px;height:50px;line-height:50px;transform-origin: 0% 0%; transform:rotate(-60deg)\">"
                    << year << "</div>";
                --year;
            break;
            }
        }
        fcout << "</div></div>";
    }

    void daily_overview(Database &database, Session &session,
            Request &request, OptsMap const &config, std::ostream &fcout)
    {
        auto it = config.find("asset-dir");
        std::string assetpath;
        if (it != config.end()) assetpath = it->second;

        std::string monthstr = request.get("relative_month");
        uint64_t relative_month;
        if (!parse_unsigned(monthstr, relative_month)) relative_month = 0;
        fcout << "<h2>Daily overview</h2>";
        std::vector<std::pair<Category, Category::BalanceData>> data;
        std::map<Category, std::vector<Category::BalanceData>> sorted;
        Category::daily_overview(database, session.user(), data, relative_month);

        // Get last day of month
        time_t t;
        time(&t);
        tm bt;
        localtime_r(&t, &bt);
        bt.tm_mon = bt.tm_mon+1 - relative_month;
        bt.tm_mday = 0;
        mktime(&bt);

        // Display current month
        char buffer[64];
        strftime(buffer, sizeof(buffer), "%B, %Y", &bt);

        fcout << "<div class=\"date\"><span class=\"arrow\">";
        fcout << "<a href=\"?q=daily_overview&relative_month="
            << relative_month + 1
            << "\"><img alt=\"left\" width=\"32\" src=\""
            << assetpath << "/arrow-left.png\"></img></a>";
        fcout << "</span>";

        fcout << "<span style=\"height: 32px\">" << buffer << "</span>";

        if (relative_month > 0)
        {
            fcout << "<span class=\"arrow\">";
            fcout << "<a href=\"?q=daily_overview&relative_month="
                << relative_month - 1
                << "\"><img alt=\"right\" width=\"32\" src=\""
                << assetpath << "/arrow-right.png\"></img></a>";
            fcout << "</span>";

            fcout << "<span class=\"arrow\">";
            fcout << "<a href=\"?q=daily_overview&relative_month=0"
                << "\"><img alt=\"right\" width=\"32\" src=\""
                << assetpath << "/arrow-right-double.png\"></img></a>";
        }
        fcout << "</span></div>";

        if (data.empty())
        {
            fcout << "Sorry, this month is empty.";
            return;
        }

        for (auto &d : data) sorted[d.first].push_back(d.second);
        fcout << "<table><tr><th>Day</th>";
        for (auto &d : sorted) fcout << "<th>" << d.first.name() << "</th>";
        fcout << "</tr>";
        
        for (int i = bt.tm_mday; i > 0; --i)
        {
            fcout << "<tr><td>" << i << "</td>";
            for (auto &d : sorted)
            {
                bool gotbalance = false;
                for (auto &b : d.second)
                {
                    if ((int)b.interval == i)
                    {
                        gotbalance = true;
                        fcout << "<td title=\"" << b.cumulative << "\"><p class=\"expense\">";
                        if (b.expensesum == 0) fcout << "&nbsp;";
                        else fcout << b.expensesum;
                        fcout << "</p><p class=\"plan\">";
                        if (b.plannedsum == 0) fcout << "&nbsp;";
                        else fcout << b.plannedsum;
                        fcout << "</p></td>";
                        break;
                    }
                }
                if (!gotbalance) fcout << "<td><p class=\"expense\">&nbsp;</p><p class=\"plan\">&nbsp;</p></td>";
            }
            fcout << "</tr>";
        }
        fcout << "</table>";
    }

    void error404(OptsMap const &config,
            Request &request, std::ostream &fcout)
    {
        if (request.type() != RequestType::Get)
        {
            header(fcout, config, "Error 404");
            menu(fcout);
            fcout << "<div class=\"content\">";
        }
        // TODO: handle 404 gracefully
        fcout << "Error: 404";
        if (request.type() != RequestType::Get)
        {
            fcout << "</div>";
            footer(fcout);
        }
    }

    void handle_request(OptsMap const &config,
            Database &database, Session &session,
            Request &request, std::ostream &fcout)
    {
        if (request.type() == RequestType::Get)
        {
            header(fcout, config, "Main page");
            menu(fcout);
            fcout << "<div class=\"content\">";
        }

        if (request.query().empty() || request.query() == "main")
            main_page(fcout);
        else if (request.query() == "item")
            item_page(database, session, request, config, fcout);
        else if (request.query() == "item_add")
            item_add(database, session, request, fcout);
        else if (request.query() == "item_edit")
            item_edit(database, session, request, fcout);
        else if (request.query() == "item_destroy_conform")
            item_destroy_conform(request, fcout);
        else if (request.query() == "plan")
            plan_page(database, session, request, fcout);
        else if (request.query() == "planitem_add")
            planitem_add(database, session, request, fcout);
        else if (request.query() == "planitem_edit")
            planitem_edit(database, session, request, fcout);
        else if (request.query() == "planitem_destroy_conform")
            planitem_destroy_conform(request, fcout);
        else if (request.query() == "category")
            category_page(database, session, request, fcout);
        else if (request.query() == "category_add")
            category_add(database, session, request, fcout);
        else if (request.query() == "category_edit")
            category_edit(database, session, request, fcout);
        else if (request.query() == "category_destroy_conform")
            category_destroy_conform(request, fcout);
        else if (request.query() == "category_destroy")
            category_destroy(database, session, request, fcout);
        else if (request.query() == "item_destroy")
            item_destroy(database, session, request, fcout);
        else if (request.query() == "planitem_destroy")
            planitem_destroy(database, session, request, fcout);
        else if (request.query() == "diagram")
            balance_stats(database, session, request, fcout);
        else if (request.query() == "daily_overview")
            daily_overview(database, session, request, config, fcout);
        else error404(config, request, fcout); 

        if (request.type() == RequestType::Get)
        {
            fcout << "</div>";
            footer(fcout);
        }
    }

}

#endif
