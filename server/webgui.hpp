#ifndef WEBGUI_HPP_INCLUDED
#define WEBGUI_HPP_INCLUDED

#include <iostream>

#include "session.hpp"
#include "database.hpp"
#include "category.hpp"
#include "plan.hpp"
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
        fcout << "<a href=\"?q=logout\">logout</a> ";
        fcout << "</div>";
    }

    void main_page(OptsMap const &config, std::ostream &fcout)
    {
        header(fcout, config, "Main page");
        menu(fcout);
        fcout << "<h2>TODO</h2>";
        footer(fcout);
    }

    void item_page(OptsMap const &config,
            Database &database, Session &session,
            Request &request, std::ostream &fcout)
    {
        // TODO: handle limit
        (void)request;
        header(fcout, config, "Item");
        menu(fcout);
        fcout << "<a href=\"?q=item_add\">Add item</a>";
        fcout << "<h2>Item</h2>";
        std::vector<Item> items;
        Item::find_all(database, session.user(), items);
        fcout << "<table><tr><th>Név</th><th>Megjegyzés</th></tr>";
        for (auto &item : items)
        {
            fcout << "<tr>";
            fcout << "<td>" << item.amount() << "</td>";
            fcout << "<td>" << item.description() << "</td>";
            fcout << "</tr>";
        }
        fcout << "</table>";
        footer(fcout);
    }

    void category_page(OptsMap const &config,
            Database &database, Session &session,
            Request &request, std::ostream &fcout)
    {
        // TODO: handle limit
        (void)request;
        header(fcout, config, "Categories");
        menu(fcout);
        fcout << "<a href=\"?q=category_add\">Add category</a>";
        fcout << "<h2>Categories:</h2>";
        std::vector<Category> categories;
        Category::find_all(database, session.user(), categories);
        fcout << "<table><tr><th>Név</th><th>Megjegyzés</th></tr>";
        for (auto &category : categories)
        {
            fcout << "<tr>";
            fcout << "<td>" << category.name() << "</td>";
            fcout << "<td>" << category.description() << "</td>";
            fcout << "</tr>";
        }
        fcout << "</table>";
        footer(fcout);
    }

    void item_add(OptsMap const &config,
            Database &database, Session &session,
            Request &request, std::ostream &fcout)
    {
        if (request.type() == RequestType::Post)
        {
            BusinessLogic::item_add(database, session, request);
            fcout << "Location: ?q=item\r\n";
            fcout << "\r\n\r\n";
            return;
        }

        header(fcout, config, "Item add");
        menu(fcout);
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
        footer(fcout);
    }

    void plan_page(OptsMap const &config,
            Database &database, Session &session,
            Request &request, std::ostream &fcout)
    {
        // TODO: handle limit
        (void)request;
        header(fcout, config, "Plan");
        menu(fcout);
        fcout << "<a href=\"?q=planitem_add\">Add plan item</a>";
        fcout << "<h2>Plan</h2>";
        std::vector<Plan> plan;
        Plan::find_all(database, session.user(), plan);
        fcout << "<table><tr><th>Name</th><th>Description</th></tr>";
        for (auto &item : plan)
        {
            fcout << "<tr>";
            fcout << "<td>" << item.amount() << "</td>";
            fcout << "<td>" << item.description() << "</td>";
            fcout << "</tr>";
        }
        fcout << "</table>";
        footer(fcout);
    }

    void planitem_add(OptsMap const &config,
            Database &database, Session &session,
            Request &request, std::ostream &fcout)
    {
        if (request.type() == RequestType::Post)
        {
            BusinessLogic::planitem_add(database, session, request);
            fcout << "Location: ?q=plan\r\n";
            fcout << "\r\n\r\n";
            return;
        }

        header(fcout, config, "Item add");
        menu(fcout);
        fcout << "<br />";
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
        footer(fcout);
    }

    void category_add(OptsMap const &config,
            Database &database, Session &session,
            Request &request, std::ostream &fcout)
    {
        if (request.type() == RequestType::Post)
        {
            BusinessLogic::category_add(database, session, request);
            fcout << "Location: ?q=category\r\n";
            fcout << "\r\n\r\n";
            return;
        }

        header(fcout, config, "Category add");
        menu(fcout);
        fcout << "<br />";
        fcout << "<form name=\"category_add_form\"  method=\"post\" action=\"?q=category_add\">";
        fcout << "Name: <input name=\"name\">";
        fcout << "Description: <textarea rows=\"3\" name=\"description\"></textarea>";
        fcout << "<input type=\"submit\" value=\"Kategória hozzáadása\">";
        fcout << "</form>";
        footer(fcout);
    }

    void handle_request(OptsMap const &config,
            Database &database, Session &session,
            Request &request, std::ostream &fcout)
    {
        if (request.query().empty() || request.query() == "main")
            main_page(config, fcout);
        else if (request.query() == "item")
            item_page(config, database, session, request, fcout);
        else if (request.query() == "item_add")
            item_add(config, database, session, request, fcout);
        else if (request.query() == "plan")
            plan_page(config, database, session, request, fcout);
        else if (request.query() == "planitem_add")
            planitem_add(config, database, session, request, fcout);
        else if (request.query() == "category")
            category_page(config, database, session, request, fcout);
        else if (request.query() == "category_add")
            category_add(config, database, session, request, fcout);
        else
        {
            // TODO: handle 404 gracefully
            throw std::logic_error("Error 404");
        }
    }

}

#endif
