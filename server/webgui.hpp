#ifndef WEBGUI_HPP_INCLUDED
#define WEBGUI_HPP_INCLUDED

#include <iostream>

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
        std::vector<Item> items;
        Item::find_all(database, session.user(), items);
        fcout << "<table><tr><th>Név</th><th>Megjegyzés</th><th></th></tr>";
        for (auto &item : items)
        {
            fcout << "<tr>";
            fcout << "<td>" << item.amount() << "</td>";
            fcout << "<td>" << item.description() << "</td>";
            fcout << "<td><form method=\"POST\" action=\"?q=item_destroy\">";
                fcout << "<input type=\"hidden\" name=\"itemid\" value=\"" << item.id() << "\">";
                fcout << "<input type=\"submit\" value=\"Delete\"></form></td>";
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
            fcout << "<td><form method=\"POST\" action=\"?q=category_destroy\">";
                fcout << "<input type=\"hidden\" name=\"categoryid\" value=\"" << category.id() << "\">";
                fcout << "<input type=\"submit\" value=\"Delete\"></form></td>";
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
        std::vector<PlanItem> plan;
        PlanItem::find_all(database, session.user(), plan);
        fcout << "<table><tr><th>Name</th><th>Description</th><th></th></tr>";
        for (auto &item : plan)
        {
            fcout << "<tr>";
            fcout << "<td>" << item.amount() << "</td>";
            fcout << "<td>" << item.description() << "</td>";
            fcout << "<td><form method=\"POST\" action=\"?q=planitem_destroy\">";
                fcout << "<input type=\"hidden\" name=\"planitemid\" value=\"" << item.id() << "\">";
                fcout << "<input type=\"submit\" value=\"Delete\"></form></td>";
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
        else if (request.query() == "plan")
            plan_page(database, session, request, fcout);
        else if (request.query() == "planitem_add")
            planitem_add(database, session, request, fcout);
        else if (request.query() == "category")
            category_page(database, session, request, fcout);
        else if (request.query() == "category_add")
            category_add(database, session, request, fcout);
        else if (request.query() == "category_destroy")
            category_destroy(database, session, request, fcout);
        else if (request.query() == "item_destroy")
            item_destroy(database, session, request, fcout);
        else if (request.query() == "planitem_destroy")
            planitem_destroy(database, session, request, fcout);
        else error404(config, request, fcout); 

        if (request.type() == RequestType::Get)
        {
            fcout << "</div>";
            footer(fcout);
        }
    }

}

#endif
