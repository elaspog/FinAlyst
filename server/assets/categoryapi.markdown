<a class="apilink" href="categoryapi.html">Categories</a>
<a class="apilink" href="itemapi.html">Items</a>
<a class="apilink" href="planitemapi.html">Planitems</a>
<a class="apilink" href="statisticsspi.html">Statistics</a>

## Category

### List categories

List all the categories with name and description.

**Request type:** HTTP GET

**Result data:**

*Array of:*

|| **Key**      || **Type**     || **Description**      ||
|| id           || integer      || category id          ||
|| create       || datetime     || create date          ||
|| modify       || datetime     || last modification    ||
|| name         || string[127]  || category name        ||
|| description  || string[255]  || category description ||

**Example URL:**
http://myfinalyst/fcgi-bin/finalyst?q=webservice/categories

**JSON example:**

    {
        "success": true,
        "status": 200,
        "data": [
            {
                "id": 1,
                "create": "/Date(1224043200000)/",
                "modify": "/Date(1228035600000)/",
                "name": "Mobile",
                "description": "Mobile phone expenses"
            }
        ]
    }

### Create category

**Request type:** HTTP POST

**Request arguments:**

|| **Key**      || **Type**     || **Description**      ||
|| name         || string[127]  || category name        ||
|| description  || string[255]  || category description ||


**Result data:**

|| **Key**      || **Type**     || **Description**      ||
|| id           || integer      || new category id      ||
|| create       || datetime     || create date          ||
|| modify       || datetime     || last modification    ||
|| name         || string[127]  || category name        ||
|| description  || string[255]  || category description ||

**Example URL:**
http://myfinalyst/fcgi-bin/finalyst?q=webservice/category_add

**JSON example:**

    {
        "success": true,
        "status": 200,
        "data": {
            "id": 1,
            "create": "/Date(1224567700000)/",
            "modify": "/Date(1224567700000)/",
            "name": "Bill",
            "description": "Usual bills"
        }
    }


### Edit category

**Request type:** HTTP POST

**Request arguments:**

|| **Key**      || **Type**     || **Description**                 ||
|| categoryid   || integer      || category id                     ||
|| name         || string[127]  || category name (optional)        ||
|| description  || string[255]  || category description (optional) ||


**Result data:**

|| **Key**      || **Type**     || **Description**      ||
|| id           || integer      || new category id      ||
|| create       || datetime     || create date          ||
|| modify       || datetime     || last modification    ||
|| name         || string[127]  || category name        ||
|| description  || string[255]  || category description ||

**Example URL:**
http://myfinalyst/fcgi-bin/finalyst?q=webservice/category_edit

**JSON example:**

    {
        "success": true,
        "status": 200,
        "data": {
            "id": 1,
            "create": "/Date(1224567700000)/",
            "modify": "/Date(1224567700000)/",
            "name": "Cheese",
            "description": "Special and not so special cheese"
        }
    }


### Destroy category

**Request type:** HTTP POST

**Request arguments:**

|| **Key**      || **Type**     || **Description**      ||
|| categoryid   || integer      || category id          ||


**Result data:** null

**Example URL:**
http://myfinalyst/fcgi-bin/finalyst?q=webservice/category_destroy

**JSON example:**

    {
        "success": true,
        "status": 200,
        "data": null
    }
