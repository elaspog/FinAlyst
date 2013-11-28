<a class="apilink" href="categoryapi.html">Categories</a>
<a class="apilink" href="itemapi.html">Items</a>
<a class="apilink" href="planitemapi.html">Planitems</a>
<a class="apilink" href="statisticsapi.html">Statistics</a>

## Statistics

### Daily category expenses overview

Calculate plan/expense balance for the specified category for every
week/month or year in the specified timeframe. Balance **always** calculated for
**single** year, granulation (week/month/year) can be specified in the request.
By default balance statistics calculated in the current year, pass 1 (or more)
in relative_year request agrument to get statistics about last year
(or the previous years).


**Request type:** HTTP GET

**Request arguments:**

|| **Key**       || **Type**     || **Description**                                                                ||
|| categoryid    || integer      || plan item id                                                                   ||
|| granulation   || string       || week/month/year                                                                ||
|| relative_year || integer      || unsigned value, search limited this year (optional) (default: 0, current year) ||

**Result data:**

|| **Key**      || **Type**                  || **Description**       ||
|| -            || array of Result Interval  || result interval       ||

**Result Interval**

|| **Key**      || **Type** || **Description**                       ||
|| interval     || integer  || week of year/month/year               ||
|| expensesum   || integer  || sum of expense in the interval        ||
|| plannedsum   || integer  || sum of planned amount in the interval ||

**Example URL:**
http://myfinalyst/fcgi-bin/finalyst?q=webservice/balance_stats?categoryid=46

**JSON example:**

    {
	"success": true,
	"status": 200,
	"data": [
            {
                    interval: 38,
                    expensesum: 1000,
                    plannedsum: 0
            },
            {
                    interval: 40,
                    expensesum: 1000,
                    plannedsum: 1000
            },
	]
    }

### Weekly, montly, yearly statistics

UNFINISHED

**Request type:** HTTP GET

**Request arguments:**

|| **Key**        || **Type**     || **Description**                                                                  ||
|| categoryid     || integer      || plan item id                                                                     ||
|| granulation    || string       || week/month/year                                                                  ||
|| relative_month || integer      || unsigned value, search limited this month (optional) (default: 0, current month) ||

**Result data:**

|| **Key**      || **Type**                  || **Description**       ||
|| -            || array of Result Interval  || result interval       ||

**Result Interval**

|| **Key**      || **Type** || **Description**                       ||
|| interval     || integer  || week of year/month/year               ||
|| expensesum   || integer  || sum of expense in the interval        ||
|| plannedsum   || integer  || sum of planned amount in the interval ||
|| cumulative   || integer  || TODO ||

**Example URL:**
http://myfinalyst/fcgi-bin/finalyst?q=webservice/daily_overview?relative_month=1

**JSON example:**

TODO 
