<a class="apilink" href="categoryapi.html">Categories</a>
<a class="apilink" href="itemapi.html">Items</a>
<a class="apilink" href="planitemapi.html">Planitems</a>
<a class="apilink" href="statisticsapi.html">Statistics</a>

## Item

### List items

List all the items with category, amount and description.

**Request type:** HTTP GET

**Request arguments:**

|| **Key** || **Type** || **Description**                             ||
|| limit   || integer  || limit number of results elements (optional) ||
|| offset  || integer  || query offset (optional)                     ||

**Result data:**

*Array of:*

|| **Key**      || **Type**     || **Description**      ||
|| id           || integer      || item id              ||
|| create       || datetime     || create date          ||
|| modify       || datetime     || last modification    ||
|| categoryid   || integer      || category id          ||
|| amount       || integer      || money amount         ||
|| description  || string[255]  || item description     ||

**Example URL:**
http://myfinalyst/fcgi-bin/finalyst?q=webservice/items

**JSON example:**

    {
        "success": true,
        "status": 200,
        "data": [
            {
                "id": 1,
                "create": "/Date(1224043200000)/",
                "modify": "/Date(1228035600000)/",
                "categoryid": 3,
                "amount": 2000,
                "description": "Montly mobile bill"
            }
        ]
    }

### Create item

**Request type:** HTTP POST

**Request arguments:**

|| **Key**      || **Type**     || **Description**      ||
|| categoryid   || integer      || category id          ||
|| amount       || integer      || money amount         ||
|| description  || string[255]  || item description     ||

**Result data:**

|| **Key**      || **Type**     || **Description**      ||
|| id           || integer      || item id              ||
|| create       || datetime     || create date          ||
|| modify       || datetime     || last modification    ||
|| categoryid   || integer      || category id          ||
|| amount       || integer      || money amount         ||
|| description  || string[255]  || item description     ||

**Example URL:**
http://myfinalyst/fcgi-bin/finalyst?q=webservice/item_add

**JSON example:**

    {
        "success": true,
        "status": 200,
        "data": {
            "id": 1,
            "create": "/Date(1224456600000)/",
            "modify": "/Date(1224456600000)/",
            "categoryid": 3,
            "amount": 800,
            "description": "Icecream"
        }
    }

### Edit item

**Request type:** HTTP POST

**Request arguments:**

|| **Key**      || **Type**     || **Description**             ||
|| itemid       || integer      || item id                     ||
|| categoryid   || integer      || category id (optional)      ||
|| amount       || integer      || money amount (optional)     ||
|| description  || string[255]  || item description (optional) ||

**Result data:**

|| **Key**      || **Type**     || **Description**      ||
|| id           || integer      || item id              ||
|| create       || datetime     || create date          ||
|| modify       || datetime     || last modification    ||
|| categoryid   || integer      || category id          ||
|| amount       || integer      || money amount         ||
|| description  || string[255]  || item description     ||

**Example URL:**
http://myfinalyst/fcgi-bin/finalyst?q=webservice/item_edit

**JSON example:**

    {
        "success": true,
        "status": 200,
        "data": {
            "id": 1,
            "create": "/Date(1224456600000)/",
            "modify": "/Date(1224456600000)/",
            "categoryid": 3,
            "amount": 800,
            "description": "Icecream"
        }
    }

### Destroy item

**Request type:** HTTP POST

**Request arguments:**

|| **Key**      || **Type**     || **Description**      ||
|| itemid       || integer      || item id              ||


**Result data:** null

**Example URL:**
http://myfinalyst/fcgi-bin/finalyst?q=webservice/item_destroy

**JSON example:**

    {
        "success": true,
        "status": 200,
        "data": null
    }

