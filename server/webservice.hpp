#ifndef WEBSERVICE_HPP_INCLUDED
#define WEBSERVICE_HPP_INCLUDED

#include <iostream>

#include "request.hpp"
#include "session.hpp"
#include "template_common.hpp"
#include "utils.hpp"
#include "bulogic.hpp"

namespace WebService
{

    void item_add(Database &database, Session &session,
            Request &request, std::ostream &fcout)
    {
        html_content(fcout);
        fcout << "{\n";
        try {
            BusinessLogic::item_add(database, session, request);
            fcout << "\t\"sucess\": true,\n";
            fcout << "\t\"status\": 200,\n";
            // TODO: return created planitem
            fcout << "\t\"data\": null\n";
        } catch (BusinessLogic::MethodNotAllowed const &error)
        {
            fcout << "\t\"sucess\": false,\n";
            fcout << "\t\"status\": 405,\n";
            fcout << "\t\"data\": null\n";
        } catch (BusinessLogic::MalformedRequest const &error)
        {
            fcout << "\t\"sucess\": false,\n";
            fcout << "\t\"status\": 400,\n";
            fcout << "\t\"data\": null\n";
        } catch (...)
        {
            fcout << "\t\"sucess\": false,\n";
            fcout << "\t\"status\": 500,\n";
            fcout << "\t\"data\": null\n";
        }
        fcout << "}";
    }

    void items(Database &database, Session &session,
            Request &request, std::ostream &fcout)
    {
        // TODO: handle limit
        (void)request;
        html_content(fcout);
        std::vector<Item> items;
        Item::find_all(database, session.user(), items);
        fcout << "{\n";
        fcout << "\t\"sucess\": true,\n";
        fcout << "\t\"status\": 200,\n";
        fcout << "\t\"data\": [\n";
        unsigned count = 0;
        for (auto &item : items)
        {
            count++;
            fcout << "\t\t{\n";
            fcout << "\t\t\t\"id\": \"" << item.id() << "\",\n";
            fcout << "\t\t\t\"create\": \"/Date(" << item.create()*1000 << ")/\",\n";
            fcout << "\t\t\t\"modify\": \"/Date(" << item.modify()*1000 << ")/\",\n";
            fcout << "\t\t\t\"categoryid\": \"" << item.category().id() << "\",\n";
            fcout << "\t\t\t\"amount\": \"" << item.amount() << "\",\n";
            fcout << "\t\t\t\"description\": \"" << item.description() << "\"\n";
            fcout << "\t\t}";
            if (count < items.size()) fcout << ",";
            fcout << std::endl;
        }
        fcout << "\t]\n";
        fcout << "}";
    }

    void planitem_add(Database &database, Session &session,
            Request &request, std::ostream &fcout)
    {
        html_content(fcout);
        fcout << "{\n";
        try {
            BusinessLogic::planitem_add(database, session, request);
            fcout << "\t\"sucess\": true,\n";
            fcout << "\t\"status\": 200,\n";
            // TODO: return created planitem
            fcout << "\t\"data\": null\n";
        } catch (BusinessLogic::MethodNotAllowed const &error)
        {
            fcout << "\t\"sucess\": false,\n";
            fcout << "\t\"status\": 405,\n";
            fcout << "\t\"data\": null\n";
        } catch (BusinessLogic::MalformedRequest const &error)
        {
            fcout << "\t\"sucess\": false,\n";
            fcout << "\t\"status\": 400,\n";
            fcout << "\t\"data\": null\n";
        } catch (...)
        {
            fcout << "\t\"sucess\": false,\n";
            fcout << "\t\"status\": 500,\n";
            fcout << "\t\"data\": null\n";
        }
        fcout << "}";
    }

    void planitems(Database &database, Session &session,
            Request &request, std::ostream &fcout)
    {
        // TODO: handle limit
        (void)request;
        html_content(fcout);
        std::vector<PlanItem> planitems;
        PlanItem::find_all(database, session.user(), planitems);
        fcout << "{\n";
        fcout << "\t\"sucess\": true,\n";
        fcout << "\t\"status\": 200,\n";
        fcout << "\t\"data\": [\n";
        unsigned count = 0;
        for (auto &item : planitems)
        {
            count++;
            fcout << "\t\t{\n";
            fcout << "\t\t\t\"id\": \"" << item.id() << "\",\n";
            fcout << "\t\t\t\"create\": \"/Date(" << item.create()*1000 << ")/\",\n";
            fcout << "\t\t\t\"modify\": \"/Date(" << item.modify()*1000 << ")/\",\n";
            fcout << "\t\t\t\"categoryid\": \"" << item.category().id() << "\",\n";
            fcout << "\t\t\t\"amount\": \"" << item.amount() << "\",\n";
            fcout << "\t\t\t\"description\": \"" << item.description() << "\"\n";
            fcout << "\t\t}";
            if (count < planitems.size()) fcout << ",";
            fcout << std::endl;
        }
        fcout << "\t]\n";
        fcout << "}";
    }

    void categories(Database &database, Session &session,
            Request &request, std::ostream &fcout)
    {
        // TODO: handle limit
        (void)request;
        html_content(fcout);
        std::vector<Category> categories;
        Category::find_all(database, session.user(), categories);
        fcout << "{\n";
        fcout << "\t\"sucess\": true,\n";
        fcout << "\t\"status\": 200,\n";
        fcout << "\t\"data\": [\n";
        unsigned count = 0;
        for (auto &category : categories)
        {
            count++;
            fcout << "\t\t{\n";
            fcout << "\t\t\t\"id\": \"" << category.id() << "\",\n";
            fcout << "\t\t\t\"create\": \"/Date(" << category.create()*1000 << ")/\",\n";
            fcout << "\t\t\t\"modify\": \"/Date(" << category.modify()*1000 << ")/\",\n";
            fcout << "\t\t\t\"name\": \"" << category.name() << "\",\n";
            fcout << "\t\t\t\"description\": \"" << category.description() << "\"\n";
            fcout << "\t\t}";
            if (count < categories.size()) fcout << ",";
            fcout << std::endl;
        }
        fcout << "\t]\n";
        fcout << "}";
    }

    void category_add(Database &database, Session &session,
            Request &request, std::ostream &fcout)
    {
        html_content(fcout);
        fcout << "{\n";
        try {
            BusinessLogic::category_add(database, session, request);
            fcout << "\t\"sucess\": true,\n";
            fcout << "\t\"status\": 200,\n";
            // TODO: return created planitem
            fcout << "\t\"data\": null\n";
        } catch (BusinessLogic::MethodNotAllowed const &error)
        {
            fcout << "\t\"sucess\": false,\n";
            fcout << "\t\"status\": 405,\n";
            fcout << "\t\"data\": null\n";
        } catch (BusinessLogic::MalformedRequest const &error)
        {
            fcout << "\t\"sucess\": false,\n";
            fcout << "\t\"status\": 400,\n";
            fcout << "\t\"data\": null\n";
        } catch (...)
        {
            fcout << "\t\"sucess\": false,\n";
            fcout << "\t\"status\": 500,\n";
            fcout << "\t\"data\": null\n";
        }
        fcout << "}";
    }

    void handle_request(Database &database,
            Session &session,
            Request &request, std::ostream &fcout)
    {
        std::string query = request.query();
        if (query == "webservice/items")
            items(database, session, request, fcout);
        else if (query == "webservice/item_add")
            item_add(database, session, request, fcout);
        else if (query == "webservice/planitems")
            planitems(database, session, request, fcout);
        else if (query == "webservice/planitem_add")
            planitem_add(database, session, request, fcout);
        else if (query == "webservice/categories")
            categories(database, session, request, fcout);
        else if (query == "webservice/category_add")
            category_add(database, session, request, fcout);
        else
        {
            // TODO: handle 404 gracefully
            throw std::logic_error("Error 404");
        }
    }

}

#endif
