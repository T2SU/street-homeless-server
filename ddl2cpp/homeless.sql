-- MariaDB dump 10.17  Distrib 10.4.12-MariaDB, for Win64 (AMD64)
--
-- Host: localhost    Database: homeless
-- ------------------------------------------------------
-- Server version	10.4.12-MariaDB

/*!40101 SET @OLD_CHARACTER_SET_CLIENT=@@CHARACTER_SET_CLIENT */;
/*!40101 SET @OLD_CHARACTER_SET_RESULTS=@@CHARACTER_SET_RESULTS */;
/*!40101 SET @OLD_COLLATION_CONNECTION=@@COLLATION_CONNECTION */;
/*!40101 SET NAMES utf8 */;
/*!40103 SET @OLD_TIME_ZONE=@@TIME_ZONE */;
/*!40103 SET TIME_ZONE='+00:00' */;
/*!40014 SET @OLD_UNIQUE_CHECKS=@@UNIQUE_CHECKS, UNIQUE_CHECKS=0 */;
/*!40014 SET @OLD_FOREIGN_KEY_CHECKS=@@FOREIGN_KEY_CHECKS, FOREIGN_KEY_CHECKS=0 */;
/*!40101 SET @OLD_SQL_MODE=@@SQL_MODE, SQL_MODE='NO_AUTO_VALUE_ON_ZERO' */;
/*!40111 SET @OLD_SQL_NOTES=@@SQL_NOTES, SQL_NOTES=0 */;

--
-- Table structure for table `characters`
--

DROP TABLE IF EXISTS `characters`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `characters` (
  `pid` bigint(20) unsigned NOT NULL AUTO_INCREMENT,
  `uid` bigint(20) unsigned NOT NULL,
  `name` varchar(20) NOT NULL,
  `health` int(11) NOT NULL,
  `tiredness` int(11) NOT NULL,
  `max_health` int(11) NOT NULL,
  `max_tiredness` int(11) NOT NULL,
  `hair` int(11) NOT NULL,
  `hair_color` int(11) NOT NULL,
  `shirt` int(11) NOT NULL,
  `pants` int(11) NOT NULL,
  `shoes` int(11) NOT NULL,
  `hat` int(11) NOT NULL,
  `beard` int(11) NOT NULL,
  `gender` int(11) NOT NULL,
  `map` varchar(30) NOT NULL DEFAULT '',
  `starting_point` varchar(30) NOT NULL DEFAULT '',
  `fat` float NOT NULL,
  `muscle` float NOT NULL,
  `slimness` float NOT NULL,
  `breast` float NOT NULL,
  `money` int(11) NOT NULL,
  `created` datetime NOT NULL DEFAULT current_timestamp(),
  `last_connected` datetime NOT NULL DEFAULT current_timestamp(),
  PRIMARY KEY (`pid`),
  UNIQUE KEY `name` (`name`),
  KEY `money` (`money`),
  KEY `uid` (`uid`),
  CONSTRAINT `FK_characters_users` FOREIGN KEY (`uid`) REFERENCES `users` (`uid`) ON DELETE CASCADE ON UPDATE CASCADE
) ENGINE=InnoDB AUTO_INCREMENT=15 DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `characters`
--

LOCK TABLES `characters` WRITE;
/*!40000 ALTER TABLE `characters` DISABLE KEYS */;
INSERT INTO `characters` VALUES (11,1,'배점례',100,0,100,100,3,11438216,5,4,4,-1,-1,1,'','',0,0,100,70.7527,0,'2023-05-13 04:25:49','2023-05-13 04:25:49'),(12,1,'가난의용사손상식',100,0,100,100,2,3026478,6,6,6,2,6,0,'','',100,13.7908,0.5,0.5,0,'2023-05-13 05:51:46','2023-05-13 05:51:46'),(14,1,'계란도둑김철남',100,0,100,100,4,16777215,1,9,8,-1,5,0,'','',67.8967,66.11,0.5,0.5,0,'2023-05-13 09:30:29','2023-05-13 09:30:29');
/*!40000 ALTER TABLE `characters` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `inventory`
--

DROP TABLE IF EXISTS `inventory`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `inventory` (
  `sn` bigint(20) unsigned NOT NULL,
  `pid` bigint(20) unsigned NOT NULL,
  `itemid` int(11) NOT NULL,
  `amount` int(11) NOT NULL,
  PRIMARY KEY (`sn`),
  KEY `pid` (`pid`),
  CONSTRAINT `FK_inventory_characters` FOREIGN KEY (`pid`) REFERENCES `characters` (`pid`) ON DELETE CASCADE ON UPDATE CASCADE
) ENGINE=InnoDB DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `inventory`
--

LOCK TABLES `inventory` WRITE;
/*!40000 ALTER TABLE `inventory` DISABLE KEYS */;
/*!40000 ALTER TABLE `inventory` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `users`
--

DROP TABLE IF EXISTS `users`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `users` (
  `uid` bigint(20) unsigned NOT NULL AUTO_INCREMENT,
  `name` varchar(20) NOT NULL,
  `password` varchar(128) NOT NULL,
  `salt` varchar(32) NOT NULL,
  `created` datetime NOT NULL DEFAULT current_timestamp(),
  `last_connected` datetime NOT NULL DEFAULT current_timestamp(),
  `session_ip` varchar(20) DEFAULT NULL,
  `device_id` text DEFAULT NULL,
  PRIMARY KEY (`uid`),
  UNIQUE KEY `name` (`name`),
  KEY `last_connected` (`last_connected`),
  KEY `created` (`created`)
) ENGINE=InnoDB AUTO_INCREMENT=2 DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `users`
--

LOCK TABLES `users` WRITE;
/*!40000 ALTER TABLE `users` DISABLE KEYS */;
INSERT INTO `users` VALUES (1,'test','125d6d03b32c84d492747f79cf0bf6e179d287f341384eb5d6d3197525ad6be8e6df0116032935698f99a09e265073d1d6c32c274591bf1d0a20ad67cba921bc','test','2023-05-10 14:34:49','2023-05-13 02:57:25',NULL,NULL);
/*!40000 ALTER TABLE `users` ENABLE KEYS */;
UNLOCK TABLES;
/*!40103 SET TIME_ZONE=@OLD_TIME_ZONE */;

/*!40101 SET SQL_MODE=@OLD_SQL_MODE */;
/*!40014 SET FOREIGN_KEY_CHECKS=@OLD_FOREIGN_KEY_CHECKS */;
/*!40014 SET UNIQUE_CHECKS=@OLD_UNIQUE_CHECKS */;
/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
/*!40101 SET CHARACTER_SET_RESULTS=@OLD_CHARACTER_SET_RESULTS */;
/*!40101 SET COLLATION_CONNECTION=@OLD_COLLATION_CONNECTION */;
/*!40111 SET SQL_NOTES=@OLD_SQL_NOTES */;

-- Dump completed on 2023-05-14  9:03:30
