-- MySQL dump 10.13  Distrib 8.0.27, for Win64 (x86_64)
--
-- Host: localhost    Database: mmotee
-- ------------------------------------------------------
-- Server version	8.0.27

/*!40101 SET @OLD_CHARACTER_SET_CLIENT=@@CHARACTER_SET_CLIENT */;
/*!40101 SET @OLD_CHARACTER_SET_RESULTS=@@CHARACTER_SET_RESULTS */;
/*!40101 SET @OLD_COLLATION_CONNECTION=@@COLLATION_CONNECTION */;
/*!50503 SET NAMES utf8mb4 */;
/*!40103 SET @OLD_TIME_ZONE=@@TIME_ZONE */;
/*!40103 SET TIME_ZONE='+00:00' */;
/*!40014 SET @OLD_UNIQUE_CHECKS=@@UNIQUE_CHECKS, UNIQUE_CHECKS=0 */;
/*!40014 SET @OLD_FOREIGN_KEY_CHECKS=@@FOREIGN_KEY_CHECKS, FOREIGN_KEY_CHECKS=0 */;
/*!40101 SET @OLD_SQL_MODE=@@SQL_MODE, SQL_MODE='NO_AUTO_VALUE_ON_ZERO' */;
/*!40111 SET @OLD_SQL_NOTES=@@SQL_NOTES, SQL_NOTES=0 */;

--
-- Table structure for table `tw_clans`
--

DROP TABLE IF EXISTS `tw_clans`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!50503 SET character_set_client = utf8mb4 */;
CREATE TABLE `tw_clans` (
  `ClanID` int NOT NULL AUTO_INCREMENT,
  `Clanname` varchar(64) CHARACTER SET utf8 COLLATE utf8_bin NOT NULL DEFAULT 'NOPE',
  `LeaderName` varchar(64) CHARACTER SET utf8 COLLATE utf8_bin NOT NULL DEFAULT 'NOPE',
  `LeaderID` int NOT NULL,
  `Level` int NOT NULL DEFAULT '1',
  `Exp` int NOT NULL DEFAULT '0',
  `MaxNum` int NOT NULL DEFAULT '2',
  `Money` int NOT NULL DEFAULT '0',
  `Relevance` int NOT NULL DEFAULT '0',
  `MoneyAdd` int NOT NULL DEFAULT '0',
  `ExpAdd` int NOT NULL DEFAULT '0',
  `SpawnHouse` tinyint NOT NULL DEFAULT '0',
  `ChairHouse` int NOT NULL DEFAULT '0',
  `CreateDate` date NOT NULL DEFAULT '1000-01-01',
  PRIMARY KEY (`ClanID`),
  KEY `ClanID` (`ClanID`),
  KEY `Clanname` (`Clanname`),
  KEY `Level` (`Level`)
) ENGINE=InnoDB AUTO_INCREMENT=71 DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_0900_ai_ci;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `tw_clans`
--

LOCK TABLES `tw_clans` WRITE;
/*!40000 ALTER TABLE `tw_clans` DISABLE KEYS */;
/*!40000 ALTER TABLE `tw_clans` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `tw_mail`
--

DROP TABLE IF EXISTS `tw_mail`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!50503 SET character_set_client = utf8mb4 */;
CREATE TABLE `tw_mail` (
  `ID` int NOT NULL AUTO_INCREMENT,
  `IDOwner` int NOT NULL,
  `TextMail` varchar(64) NOT NULL DEFAULT 'No text in this mail',
  `ItemID` int DEFAULT NULL,
  `ItemCount` int NOT NULL,
  PRIMARY KEY (`ID`),
  KEY `IDOwner` (`IDOwner`)
) ENGINE=InnoDB AUTO_INCREMENT=70 DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_0900_ai_ci;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `tw_mail`
--

LOCK TABLES `tw_mail` WRITE;
/*!40000 ALTER TABLE `tw_mail` DISABLE KEYS */;
/*!40000 ALTER TABLE `tw_mail` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `tw_materials`
--

DROP TABLE IF EXISTS `tw_materials`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!50503 SET character_set_client = utf8mb4 */;
CREATE TABLE `tw_materials` (
  `ID` int NOT NULL AUTO_INCREMENT,
  `Materials` int NOT NULL DEFAULT '0',
  PRIMARY KEY (`ID`)
) ENGINE=InnoDB AUTO_INCREMENT=4 DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_0900_ai_ci;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `tw_materials`
--

LOCK TABLES `tw_materials` WRITE;
/*!40000 ALTER TABLE `tw_materials` DISABLE KEYS */;
/*!40000 ALTER TABLE `tw_materials` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `tw_uclass`
--

DROP TABLE IF EXISTS `tw_uclass`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!50503 SET character_set_client = utf8mb4 */;
CREATE TABLE `tw_uclass` (
  `UserID` int NOT NULL,
  `Username` varchar(64) CHARACTER SET utf8 COLLATE utf8_bin NOT NULL,
  `Upgrade` int NOT NULL DEFAULT '0',
  `SkillPoint` int NOT NULL DEFAULT '0',
  `Damage` int NOT NULL DEFAULT '0',
  `Speed` int NOT NULL DEFAULT '0',
  `Health` int NOT NULL DEFAULT '0',
  `HPRegen` int NOT NULL DEFAULT '0',
  `AmmoRegen` int NOT NULL DEFAULT '0',
  `Ammo` int NOT NULL DEFAULT '0',
  `Spray` int NOT NULL DEFAULT '0',
  `Mana` int NOT NULL DEFAULT '0',
  `HammerRange` int NOT NULL DEFAULT '0',
  `Pasive2` int NOT NULL DEFAULT '0',
  PRIMARY KEY (`UserID`),
  KEY `Username` (`Username`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_0900_ai_ci;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `tw_uclass`
--

LOCK TABLES `tw_uclass` WRITE;
/*!40000 ALTER TABLE `tw_uclass` DISABLE KEYS */;
/*!40000 ALTER TABLE `tw_uclass` ENABLE KEYS */;
UNLOCK TABLES;


DROP TABLE IF EXISTS `tw_uitems`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!50503 SET character_set_client = utf8mb4 */;
CREATE TABLE `tw_uitems` (
  `it_id` bigint unsigned NOT NULL AUTO_INCREMENT,
  `il_id` int unsigned NOT NULL,
  `item_type` int NOT NULL,
  `item_owner` bigint NOT NULL DEFAULT '0',
  `item_settings` smallint NOT NULL DEFAULT '0',
  `item_count` int NOT NULL DEFAULT '0',
  `item_enchant` int NOT NULL DEFAULT '0',
  PRIMARY KEY (`it_id`),
  KEY `item_owner` (`item_owner`),
  KEY `item_type` (`item_type`)
) ENGINE=InnoDB AUTO_INCREMENT=73064 DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_0900_ai_ci;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `tw_uitems`
--

LOCK TABLES `tw_uitems` WRITE;
/*!40000 ALTER TABLE `tw_uitems` DISABLE KEYS */;
/*!40000 ALTER TABLE `tw_uitems` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `tw_users`
--

DROP TABLE IF EXISTS `tw_users`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!50503 SET character_set_client = utf8mb4 */;
CREATE TABLE `tw_users` (
  `UserId` int NOT NULL AUTO_INCREMENT,
  `Username` varchar(64) CHARACTER SET utf8 COLLATE utf8_bin NOT NULL,
  `Nick` varchar(32) CHARACTER SET utf8 COLLATE utf8_bin NOT NULL,
  `Email` varchar(64) CHARACTER SET utf8 COLLATE utf8_bin NOT NULL,
  `PasswordHash` varchar(64) CHARACTER SET utf8 COLLATE utf8_bin NOT NULL,
  `Level` int NOT NULL DEFAULT '1',
  `Exp` int NOT NULL DEFAULT '0',
  `Money` bigint NOT NULL DEFAULT '100',
  `Gold` bigint NOT NULL DEFAULT '0',
  `Donate` int NOT NULL DEFAULT '0',
  `Rel` int NOT NULL DEFAULT '0',
  `Jail` tinyint(1) NOT NULL DEFAULT '0',
  `Class` int NOT NULL DEFAULT '0',
  `ClanAdded` bigint NOT NULL DEFAULT '0',
  `ClanID` int NOT NULL DEFAULT '0',
  `Quest` int NOT NULL DEFAULT '1',
  `QuestDat` int NOT NULL DEFAULT '0',
  `Seccurity` tinyint NOT NULL DEFAULT '0',
  `Killing` int NOT NULL DEFAULT '0',
  `WinArea` int NOT NULL DEFAULT '0',
  `SettingsChat` int NOT NULL DEFAULT '0',
  `SettingsDrop` int NOT NULL DEFAULT '0',
  `RegisterDate` datetime NOT NULL,
  `RegisterIp` varchar(64) NOT NULL,
  `DailyQuest1` int NOT NULL DEFAULT '1',
  `DailyQuest2` int NOT NULL DEFAULT '1',
  `DailyQuest3` int NOT NULL DEFAULT '1',
  PRIMARY KEY (`UserId`),
  KEY `UserId` (`UserId`),
  KEY `Username` (`Username`),
  KEY `UserId_2` (`UserId`)
) ENGINE=InnoDB AUTO_INCREMENT=2467 DEFAULT CHARSET=utf8mb3;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `tw_users`
--

LOCK TABLES `tw_users` WRITE;
/*!40000 ALTER TABLE `tw_users` DISABLE KEYS */;
/*!40000 ALTER TABLE `tw_users` ENABLE KEYS */;
UNLOCK TABLES;
/*!40103 SET TIME_ZONE=@OLD_TIME_ZONE */;

/*!40101 SET SQL_MODE=@OLD_SQL_MODE */;
/*!40014 SET FOREIGN_KEY_CHECKS=@OLD_FOREIGN_KEY_CHECKS */;
/*!40014 SET UNIQUE_CHECKS=@OLD_UNIQUE_CHECKS */;
/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
/*!40101 SET CHARACTER_SET_RESULTS=@OLD_CHARACTER_SET_RESULTS */;
/*!40101 SET COLLATION_CONNECTION=@OLD_COLLATION_CONNECTION */;
/*!40111 SET SQL_NOTES=@OLD_SQL_NOTES */;

-- Dump completed on 2022-06-01  0:39:09
