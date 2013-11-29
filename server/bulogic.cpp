#include "bulogic.hpp"

namespace BusinessLogic
{

    Item item_add(Database &database, Session &session, Request &request)
    {
        if (request.type() != RequestType::Post)
            throw MethodNotAllowed("item_add only accepts POST request");
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
            throw AccessDenied(
                    "Categoy belongs to a different user!");
        }
        Item n(database, session.user(), category,
                amount, request.post("description"));
        n.save();
        return n;
    }

    PlanItem planitem_add(Database &database, Session &session, Request &request)
    {
        if (request.type() != RequestType::Post)
        throw MethodNotAllowed("planitem_add only accepts POST request");
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
            throw AccessDenied(
                    "Categoy belongs to a different user!");
        }
        PlanItem n(database, session.user(), category,
                amount, request.post("description"));
        n.save();
        return n;
    }

    Category category_add(Database &database, Session &session, Request &request)
    {
        if (request.type() != RequestType::Post)
            throw MethodNotAllowed("category_add only accepts POST request");
        std::string name = request.post("name");
        if (name.empty())
            throw BusinessLogic::MalformedRequest("Category name can't be empty!!");
        Category n(database, session.user(),
                name, request.post("description"));
        n.save();
        return n;
    }

    Category category_edit(Database &database, Session &session, Request &request)
    {
        if (request.type() != RequestType::Post)
            throw MethodNotAllowed("category_add only accepts POST request");
        std::string id_str = request.post("categoryid");
        uint64_t categoryid;
        if (id_str.empty() || !parse_unsigned(id_str, categoryid))
            throw MalformedRequest("Invalid or missing categoryid!");
        
        Category c = Category::find(database, categoryid);
        if (!c.valid())
            throw MalformedRequest("Category does not exists!");
        if (c.user().id() != session.user().id())
            throw AccessDenied("Can't change category, "
                    "it belongs to different user!");

        std::string name;
        if (request.post("name", name))
        {
            if (name.empty())
                throw MalformedRequest("Category name can't be empty!");
            c.name(name);
        }
       
        std::string description;
        if (request.post("description", description))
            c.description(description);
        c.save();

        return c;
    }

    Item item_edit(Database &database, Session &session, Request &request)
    {
        if (request.type() != RequestType::Post)
            throw MethodNotAllowed("item_edit only accepts POST request");
        std::string id_str = request.post("itemid");
        uint64_t itemid;
        if (id_str.empty() || !parse_unsigned(id_str, itemid))
            throw MalformedRequest("Invalid or missing itemid!");
        Item c = Item::find(database, itemid);
        if (!c.valid())
            throw MalformedRequest("Item does not exists!");
        if (c.user().id() != session.user().id())
            throw AccessDenied("Can't change item, "
                    "it belongs to different user!");

        std::string amount_str;
        if (request.post("amount", amount_str))
        {
            uint64_t amount;
            if (amount_str.empty() || !parse_unsigned(amount_str, amount))
                throw MalformedRequest("Amount is not a number!");
            c.amount(amount);
        }

        std::string categoryid_str;
        if (request.post("categoryid", categoryid_str))
        {
            uint64_t categoryid;
            if (id_str.empty() || !parse_unsigned(categoryid_str, categoryid))
                throw MalformedRequest("Invalid categoryid!");
            Category category = Category::find(database, categoryid);
            if (!category.valid())
                throw MalformedRequest("Category does not exists!");
            c.category(category);
        }

        std::string description;
        if (request.post("description", description))
            c.description(description);

        c.save();
        return c;
    }

    PlanItem planitem_edit(Database &database, Session &session, Request &request)
    {
        if (request.type() != RequestType::Post)
            throw MethodNotAllowed("planitem_edit only accepts POST request");
        std::string id_str = request.post("planitemid");
        uint64_t planitemid;
        if (id_str.empty() || !parse_unsigned(id_str, planitemid))
            throw MalformedRequest("Invalid or missing planitemid!");
        PlanItem c = PlanItem::find(database, planitemid);
        if (!c.valid())
            throw MalformedRequest("Planitem does not exists!");
        if (c.user().id() != session.user().id())
                throw AccessDenied("Can't change planitem, "
                        "it belongs to different user!");

        std::string amount_str;
        if (request.post("amount", amount_str))
        {
            uint64_t amount;
            if (amount_str.empty() || !parse_unsigned(amount_str, amount))
                throw MalformedRequest("Amount is not a number!");
            c.amount(amount);
        }

        std::string categoryid_str;
        if (request.post("categoryid", categoryid_str))
        {
            uint64_t categoryid;
            if (id_str.empty() || !parse_unsigned(categoryid_str, categoryid))
                throw MalformedRequest("Invalid categoryid!");
            Category category = Category::find(database, categoryid);
            if (!category.valid())
                throw MalformedRequest("Category does not exists!");
            c.category(category);
        }

        std::string description;
        if (request.post("description", description))
            c.description(description);

        c.save();
        return c;
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

    /*void balance(Database &database, Session &session, Request &request,
            std::vector<std::pair<Category, Category::BalanceData>> &data)
    {
        (void)session; (void)request;
        Category::balance(database, data);
    }*/

    void balance_stats(Database &database, Session &session, Request &request,
            std::vector<Category::BalanceData> &data)
    {
        std::string id_str = request.get("categoryid");
        uint64_t categoryid;
        if (id_str.empty() || !parse_unsigned(id_str, categoryid))
            throw MalformedRequest("Invalid or missing categoryid!");
        
        std::string granulation = request.get("granulation");
        Category::StatGranulation gran = Category::StatGranulation::weekly;
        if (granulation == "week")
            gran = Category::StatGranulation::weekly;
        else if (granulation == "month")
            gran = Category::StatGranulation::monthly;
        else if (granulation == "year")
            gran = Category::StatGranulation::yearly;
        else
            throw MalformedRequest("Invalid or missing granulation!");

        Category c = Category::find(database, categoryid);
        if (!c.valid())
            throw MalformedRequest("Category does not exists!");
        if (c.user().id() != session.user().id())
            throw AccessDenied("Can't list category stats, "
                    "it belongs to different user!");

        std::string yearstr;
        uint64_t relative_year = 0;
        if (request.get("relative_year", yearstr))
        {
            if (yearstr.empty() || !parse_unsigned(yearstr, relative_year))
                throw MalformedRequest("Invalid relative_year!");
        }

        c.balance_stats(data, gran, relative_year);
    }

    void daily_overview(Database &database, Session &session, Request &request,
            std::vector<std::pair<Category, Category::BalanceData>> &data)
    {
        std::string monthstr;
        uint64_t relative_month = 0;
        if (request.get("relative_month", monthstr))
        {
            if (monthstr.empty() || !parse_unsigned(monthstr, relative_month))
                throw MalformedRequest("Invalid relative_month!");
        }

        Category::daily_overview(database, session.user(),
                data, relative_month);
    }



}
