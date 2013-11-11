#ifndef BULOGIC_HPP_INCLUDED
#define BULOGIC_HPP_INCLUDED

#include <stdexcept>

#include "database.hpp"
#include "session.hpp"
#include "request.hpp"
#include "item.hpp"
#include "planitem.hpp"
#include "category.hpp"
#include "utils.hpp"

namespace BusinessLogic
{

    struct MethodNotAllowed : std::invalid_argument
    {
        MethodNotAllowed(std::string const &message) :
            std::invalid_argument(message)
        {}
    };

    struct MalformedRequest : std::invalid_argument
    {
        MalformedRequest(std::string const &message) :
            std::invalid_argument(message)
        {}
    };

    struct AccessDenied : std::invalid_argument
    {
        AccessDenied(std::string const &message) :
            std::invalid_argument(message)
        {}
    };

    void item_add(Database &database, Session &session, Request &request)
    {
        if (request.type() == RequestType::Post)
        {
            std::string amountstr = request.post("amount");
            uint64_t amount;
            uint64_t categoryid;
            if (amountstr.empty() || !parse_unsigned(amountstr, amount))
                throw MalformedRequest("Invalid amount!");
            if (!parse_unsigned(request.post("categoryid"), categoryid))
                throw MalformedRequest("Invalid categoryid!");
            Category category = Category::find(database, categoryid);
            if (!category.valid())
                throw MalformedRequest("Category does not exits!");
            if (category.user().id() != session.user().id())
            {
                // TODO: security alert
                throw MalformedRequest(
                        "Categoy belongs to a different user!");
            }
            Item n(database, session.user(), category,
                    amount, request.post("description"));
            n.save();
        } else {
            throw MethodNotAllowed("item_add only accepts POST request");
        }
    }

    void planitem_add(Database &database, Session &session, Request &request)
    {
        if (request.type() == RequestType::Post)
        {
            std::string amountstr = request.post("amount");
            uint64_t amount;
            uint64_t categoryid;
            if (amountstr.empty() || !parse_unsigned(amountstr, amount))
                throw MalformedRequest("Invalid amount!");
            if (!parse_unsigned(request.post("categoryid"), categoryid))
                throw MalformedRequest("Invalid categoryid!");
            Category category = Category::find(database, categoryid);
            if (!category.valid())
                throw MalformedRequest("Category does not exits!");
            if (category.user().id() != session.user().id())
            {
                // TODO: security alert
                throw MalformedRequest(
                        "Categoy belongs to a different user!");
            }
            PlanItem n(database, session.user(), category,
                    amount, request.post("description"));
            n.save();
        } else {
            throw MethodNotAllowed("planitem_add only accepts POST request");
        }
    }

    void category_add(Database &database, Session &session, Request &request)
    {
        if (request.type() == RequestType::Post)
        {
            std::string name = request.post("name");
            if (name.empty())
                throw BusinessLogic::MalformedRequest("Category name can't be empty!!");
            Category n(database, session.user(),
                    name, request.post("description"));
            n.save();
        } else {
            throw MethodNotAllowed("category_add only accepts POST request");
        }
    }

    void category_edit(Database &database, Session &session, Request &request)
    {
        if (request.type() == RequestType::Post)
        {
            std::string id_str = request.post("categoryid");
            uint64_t categoryid;
            if (id_str.empty() || !parse_unsigned(id_str, categoryid))
                throw MalformedRequest("Invalid or missing categoryid!");
            std::string name = request.post("name");
            if (name.empty())
                throw MalformedRequest("Category name can't be empty!");
            Category c = Category::find(database, categoryid);
            if (!c.valid())
                throw MalformedRequest("Category does not exists!");
            if (c.user().id() != session.user().id())
                throw AccessDenied("Can't change category, "
                        "it belongs to different user!");
            LOG_DEBUG("Name: %s desc: %s", c.name().c_str(), c.description().c_str());
            c.name(name);
            c.description(request.post("description"));
            c.save();
        } else {
            throw MethodNotAllowed("category_add only accepts POST request");
        }
    }

    void category_destroy(Database &database, Session &session, Request &request)
    {
        if (request.type() == RequestType::Post)
        {
            std::string id_str = request.post("categoryid");
            uint64_t categoryid;
            if (id_str.empty() || !parse_unsigned(id_str, categoryid))
                throw MalformedRequest("Invalid or missing categoryid!");
            Category::destroy(database, session.user().id(), categoryid);
        } else {
            throw MethodNotAllowed("category_destroy only accepts POST request");
        }
    }

    void item_destroy(Database &database, Session &session, Request &request)
    {
        if (request.type() == RequestType::Post)
        {
            std::string id_str = request.post("itemid");
            uint64_t itemid;
            if (id_str.empty() || !parse_unsigned(id_str, itemid))
                throw MalformedRequest("Invalid or missing itemid!");
            Item::destroy(database, session.user().id(), itemid);
        } else {
            throw MethodNotAllowed("item_destroy only accepts POST request");
        }
    }

    void planitem_destroy(Database &database, Session &session, Request &request)
    {
        if (request.type() == RequestType::Post)
        {
            std::string id_str = request.post("planitemid");
            uint64_t planitemid;
            if (id_str.empty() || !parse_unsigned(id_str, planitemid))
                throw MalformedRequest("Invalid or missing planitemid!");
            PlanItem::destroy(database, session.user().id(), planitemid);
        } else {
            throw MethodNotAllowed("planitem_destroy only accepts POST request");
        }
    }

}

#endif
