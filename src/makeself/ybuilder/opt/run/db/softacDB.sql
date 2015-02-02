-- create database 'softacDB'
CREATE DATABASE IF NOT EXISTS softacDB;
USE softacDB;

-- create table 'admin_user'
DROP TABLE IF EXISTS `admin_user`;
CREATE TABLE IF NOT EXISTS `admin_user` (
  `_id` int(11) NOT NULL PRIMARY KEY AUTO_INCREMENT,
  `name` varchar(64) NOT NULL,
  `passwd` varchar(64) DEFAULT '',
  `administrator` tinyint(1) DEFAULT '0',
  `email_alert` tinyint(1) DEFAULT '0',
  `email_address` varchar(64) DEFAULT '',
  `language` varchar(64) DEFAULT 'en_US',
  `note` varchar(255) DEFAULT '',
  `last_login_time` datetime DEFAULT NULL,
  `last_login_ip` varchar(64) DEFAULT '0.0.0.0'
);

-- create table 'object_active'
DROP TABLE IF EXISTS `object_active`;
CREATE TABLE `object_active` (
  `_id` int(11) NOT NULL PRIMARY KEY AUTO_INCREMENT,
  `event_time` timestamp NOT NULL DEFAULT CURRENT_TIMESTAMP,
  `event` varchar(64) NOT NULL,
  `msg` varchar(255) NOT NULL,
  `admin` varchar(64) DEFAULT NULL,
  `ap` varchar(32) DEFAULT NULL,
  `user` varchar(32) DEFAULT NULL,
  `guest` varchar(32) DEFAULT NULL
);

-- grant db user and privilege
GRANT all on *.* to AFC@localhost IDENTIFIED by "CFA";
