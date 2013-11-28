<a class="apilink" href="webserviceapi.html">Basic format</a>
<a class="apilink" href="categoryapi.html">Categories</a>
<a class="apilink" href="itemapi.html">Items</a>
<a class="apilink" href="planitemapi.html">Planitems</a>
<a class="apilink" href="statisticsapi.html">Statistics</a>

## Plan item

### List plan items

List all the plan items with category, amount and description.

**Request type:** HTTP GET

**Request arguments:**

|| **Key** || **Type** || **Description**                             ||
|| limit   || integer  || limit number of results elements (optional) ||
|| offset  || integer  || query offset (optional)                     ||

**Result data:**

|| **Key**      || **Type**     || **Description**      ||
|| id           || integer      || plan item id         ||
|| create       || datetime     || create date          ||
|| modify       || datetime     || last modification    ||
|| categoryid   || integer      || category id          ||
|| amount       || integer      || money amount         ||
|| description  || string[255]  || item description     ||

**Example URL:**
http://myfinalyst/fcgi-bin/finalyst?q=webservice/planitems

**JSON example:**

    {
        "success": true,
        "status": 200,
        "data": [
            {
                "id": 1,
                "create": "/Date(1224043200000)/",
                "modify": "/Date(1228035600000)/",
                "categoryid": 5,
                "amount": 5000,
                "description": "Got 5000Ft for birthday, I will spend it on travelling"
            }
        ]
    }

### Create planitem

**Request type:** HTTP POST

**Request arguments:**

|| **Key**      || **Type**     || **Description**      ||
|| categoryid   || integer      || category id          ||
|| amount       || integer      || money amount         ||
|| description  || string[255]  || item description     ||

**Result data:**

|| **Key**      || **Type**     || **Description**      ||
|| id           || integer      || plan item id         ||
|| create       || datetime     || create date          ||
|| modify       || datetime     || last modification    ||
|| categoryid   || integer      || category id          ||
|| amount       || integer      || money amount         ||
|| description  || string[255]  || item description     ||

**Example URL:**
http://myfinalyst/fcgi-bin/finalyst?q=webservice/planitem_add

**JSON example:**

    {
        "success": true,
        "status": 200,
        "data": {
            "id": 1,
            "create": "/Date(1224778800000)/",
            "modify": "/Date(1224778800000)/",
            "categoryid": 3,
            "amount": 10000,
            "description": "Got my wage, spend this part on sport"
        }
    }

### Edit planitem

**Request type:** HTTP POST

**Request arguments:**

|| **Key**      || **Type**     || **Description**             ||
|| planid       || integer      || plan item id                ||
|| categoryid   || integer      || category id (optional)      ||
|| amount       || integer      || money amount (optional)     ||
|| description  || string[255]  || item description (optional) ||

**Result data:**

|| **Key**      || **Type**     || **Description**      ||
|| id           || integer      || plan item id         ||
|| create       || datetime     || create date          ||
|| modify       || datetime     || last modification    ||
|| categoryid   || integer      || category id          ||
|| amount       || integer      || money amount         ||
|| description  || string[255]  || item description     ||

**Example URL:**
http://myfinalyst/fcgi-bin/finalyst?q=webservice/planitem_edit

**JSON example:**

    {
        "success": true,
        "status": 200,
        "data": {
            "id": 1,
            "create": "/Date(1224778800000)/",
            "modify": "/Date(1224778800000)/",
            "categoryid": 3,
            "amount": 10000,
            "description": "Got my wage, spend this part on sport"
        }
    }


### Destroy planitem

**Request type:** HTTP POST

**Request arguments:**

|| **Key**      || **Type**     || **Description**      ||
|| planitemid   || integer      || item id              ||


**Result data:** null

**Example URL:**
http://myfinalyst/fcgi-bin/finalyst?q=webservice/planitem_destroy

**JSON example:**

    {
        "success": true,
        "status": 200,
        "data": null
    }

