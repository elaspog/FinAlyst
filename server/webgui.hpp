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
        fcout << "<h2>TODO</h2>";

        fcout << "</div>";
        footer(fcout);
    }

    void item_page(Database &database, Session &session,
            Request &request, std::ostream &fcout)
    {
        // TODO: handle limit
        (void)request;
        fcout << "<a href=\"?q=item_add\">Add item</a>";
        fcout << "<h2>Item</h2>";
        std::vector<std::pair<Category, Item>> data;
        Item::find_all_with_category(database, session.user(), data);
        fcout << "<table><tr><th>Category</th><th>Amount</th><th>Description</th><th></th></tr>";
        for (auto &item : data)
        {
            fcout << "<tr>";
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
        fcout << "<table><tr><th>Category</th><th>Amount</th><th>Description</th><th></th></tr>";
        for (auto &item : data)
        {
            fcout << "<tr>";
            fcout << "<td>" << item.first.name() << "</td>";
            fcout << "<td>" << item.second.amount() << "</td>";
            fcout << "<td>" << item.second.description() << "</td>";
            fcout <<   "<td>";
            fcout <<     "<a href=\"?q=planitem_edit&itemid=" << item.second.id() << "\">Edit</a>";
            fcout <<     "<a rel=\"nofollow\" href=\"?q=planitem_destroy_conform&itemid=" << item.second.id() << "\">Delete</a>";
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

    void daily_overview(Database &database, Session &session,
            Request &request, std::ostream &fcout)
    {
        (void)request;
        fcout << "<h2>Daily overview</h2>";
        std::vector<std::pair<Category, Category::BalanceData>> data;
        std::map<Category, std::vector<Category::BalanceData>> sorted;
        Category::daily_overview(database, session.user(), data);
        for (auto &d : data) sorted[d.first].push_back(d.second);
        /*for (auto &d : sorted)
        {
            std::sort(d.second.begin(), d.second.end(),
                [](const Category::BalanceData &a, const Category::BalanceData &b) {
                     return a.interval > b.interval; });
        }*/
        fcout << "<table><tr><th>Day</th>";
        for (auto &d : sorted) fcout << "<th>" << d.first.name() << "</th>";
        fcout << "</tr>";
       
        // Get last day of month
        time_t t;
        time(&t);
        tm bt;
        localtime_r(&t, &bt);
        bt.tm_mon++;
        bt.tm_mday = 0;
        mktime(&bt);
        
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
            item_page(database, session, request, fcout);
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
        else if (request.query() == "daily_overview")
            daily_overview(database, session, request, fcout);
        else error404(config, request, fcout); 

        if (request.type() == RequestType::Get)
        {
            fcout << "</div>";
            footer(fcout);
        }
    }

}

#endif
