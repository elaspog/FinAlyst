#ifndef BULOGIC_HPP_INCLUDED
#define BULOGIC_HPP_INCLUDED

#include <stdexcept>

#include "database.hpp"
#include "session.hpp"
#include "request.hpp"
#include "item.hpp"
#include "plan.hpp"
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
            Plan n(database, session.user(), category,
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

}

#endif
