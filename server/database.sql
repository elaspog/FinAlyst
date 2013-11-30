-- This file is part of FinAlyst.
--
-- FinAlyst is free software: you can redistribute it and/or modify
-- it under the terms of the GNU General Public License as published by
-- the Free Software Foundation, either version 3 of the License, or
-- (at your option) any later version.
--
-- FinAlyst is distributed in the hope that it will be useful,
-- but WITHOUT ANY WARRANTY; without even the implied warranty of
-- MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
-- GNU General Public License for more details.
--
-- You should have received a copy of the GNU General Public License
-- along with FinAlyst. If not, see <http://www.gnu.org/licenses/>.

-- Create and setup tables needed to run FinAlyst server

CREATE TABLE IF NOT EXISTS `categories` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `create` datetime NOT NULL,
  `modify` datetime NOT NULL,
  `name` varchar(127) NOT NULL,
  `description` varchar(255) NOT NULL,
  `userid` int(11) NOT NULL,
  PRIMARY KEY (`id`),
  UNIQUE KEY `name` (`name`,`userid`),
  KEY `userid` (`userid`)
) DEFAULT CHARSET=utf8;

CREATE TABLE IF NOT EXISTS `items` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `create` datetime NOT NULL,
  `modify` datetime NOT NULL,
  `userid` int(11) NOT NULL,
  `categoryid` int(11) NOT NULL,
  `description` varchar(255) NOT NULL,
  `amount` int(11) NOT NULL,
  PRIMARY KEY (`id`),
  KEY `userid` (`userid`,`categoryid`),
  KEY `categoryid` (`categoryid`)
) DEFAULT CHARSET=utf8;

CREATE TABLE IF NOT EXISTS `planitems` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `userid` int(11) NOT NULL,
  `categoryid` int(11) NOT NULL,
  `create` datetime NOT NULL,
  `modify` datetime NOT NULL,
  `description` varchar(255) NOT NULL,
  `amount` int(11) NOT NULL,
  PRIMARY KEY (`id`),
  KEY `userid` (`userid`,`categoryid`),
  KEY `categoryid` (`categoryid`)
) DEFAULT CHARSET=utf8;

CREATE TABLE IF NOT EXISTS `users` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `create` datetime NOT NULL,
  `modify` datetime NOT NULL,
  `name` varchar(63) NOT NULL,
  `passdigest` varchar(128) NOT NULL,
  `passsalt` varchar(128) NOT NULL,
  `active` tinyint(1) NOT NULL DEFAULT '1',
  PRIMARY KEY (`id`)
) DEFAULT CHARSET=utf8;


ALTER TABLE `categories`
  ADD CONSTRAINT `categories_ibfk_1` FOREIGN KEY (`userid`) REFERENCES `users` (`id`);

ALTER TABLE `items`
  ADD CONSTRAINT `items_ibfk_1` FOREIGN KEY (`userid`) REFERENCES `users` (`id`),
  ADD CONSTRAINT `items_ibfk_2` FOREIGN KEY (`categoryid`) REFERENCES `categories` (`id`);

ALTER TABLE `planitems`
  ADD CONSTRAINT `planitems_ibfk_1` FOREIGN KEY (`userid`) REFERENCES `users` (`id`),
  ADD CONSTRAINT `planitems_ibfk_2` FOREIGN KEY (`categoryid`) REFERENCES `categories` (`id`);

