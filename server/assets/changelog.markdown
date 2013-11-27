# FinAlyst changelog #

## 2013. 11. 27. ##

* Fixed category/item/planitem add on webservice api

## 2013. 11. 26. ##

* Added category name for items and planitems on webgui
* Added additional logging

## 2013. 11. 25. ##

* Added weekly/monthly/yearly statistics on webservice interface

## 2013. 11. 23. ##

* Added syslog support
* Proper gcrypt initialization
* Removed useless log messages

## 2013. 11. 22. ##

* Session management refactoring
* Makefile improvements
* SessionManager supports memcached

## 2013. 11. 19. ##

* Added item, planitem change support
* Added proper result to JSON webservice, create/modify category, planitem and item

## 2013. 11. 11. ##

* Added category change support

## 2013. 11. 10. ##

* JSON string escaped
* fixed date bug on 32bit platform

## 2013. 11. 09. ##

* API docs specifiy maximum string sizes
* Refactoring in webservice
* Category/Item/Planitem destroy works on webgui nad webservice interface
* Configurable log file location

## 2013. 11. 08. ##

* fixed root login
* fixed plan item listing

## 2013. 11. 07. ##

* refactored database query handling
* fixed assert macro
* Plan class renamed to PlanItem (no changes in the JSON interface)

## 2013. 10. 30. ##

* active record objects obtaine id after insert

## 2013. 10. 28. ##

* changed markdown generatin in Makefile
* moved webservice.markdown to assets directory
* added static page display to common template functions
* added changelog
