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

    Item item_add(Database &database, Session &session, Request &request);
    PlanItem planitem_add(Database &database, Session &session, Request &request);
    Category category_add(Database &database, Session &session, Request &request);
    Category category_edit(Database &database, Session &session, Request &request);
    Item item_edit(Database &database, Session &session, Request &request);
    PlanItem planitem_edit(Database &database, Session &session, Request &request);
    void category_destroy(Database &database, Session &session, Request &request);
    void item_destroy(Database &database, Session &session, Request &request);
    void planitem_destroy(Database &database, Session &session, Request &request);

    void balance_stats(Database &database, Session &session, Request &request,
            std::vector<Category::BalanceData> &data);

}

#endif
