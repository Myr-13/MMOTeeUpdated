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
  `Exp` int NOT NULL,
  `MaxNum` int NOT NULL DEFAULT '2',
  `Money` int NOT NULL,
  `Relevance` int NOT NULL DEFAULT '0',
  `MoneyAdd` int NOT NULL DEFAULT '0',
  `ExpAdd` int NOT NULL DEFAULT '0',
  `SpawnHouse` tinyint NOT NULL DEFAULT '0',
  `ChairHouse` int NOT NULL DEFAULT '0',
  `CreateDate` date NOT NULL,
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

--
-- Table structure for table `tw_uitemlist`
--

DROP TABLE IF EXISTS `tw_uitemlist`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!50503 SET character_set_client = utf8mb4 */;
CREATE TABLE `tw_uitemlist` (
  `il_id` int unsigned NOT NULL AUTO_INCREMENT,
  `item_type` int DEFAULT '0',
  `item_name` char(50) NOT NULL,
  `item_desc` char(50) NOT NULL DEFAULT '- No description',
  PRIMARY KEY (`il_id`),
  KEY `item_type` (`item_type`)
) ENGINE=InnoDB AUTO_INCREMENT=124 DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_0900_ai_ci;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `tw_uitemlist`
--

LOCK TABLES `tw_uitemlist` WRITE;
/*!40000 ALTER TABLE `tw_uitemlist` DISABLE KEYS */;
INSERT INTO `tw_uitemlist` VALUES 
(1,4,'Money Bag','- Random get 5-20000 silvers'),
(2,3,'Quest Pig Meat','- Need for quest'),
(3,3,'Quest Kwah Head','- Need for quest'),
(4,1,'Hook Damage','- Weapon / Upgrading'),
(5,1,'Gun','- Weapon / Upgrading'),
(6,1,'Shotgun','- Weapon / Upgrading'),
(7,1,'Grenade','- Weapon / Upgrading'),
(8,1,'Laser','- Weapon / Upgrading'),
(9,2,'Amulet Cleever','This for got x2 money bag in level up'),
(10,2,'Ring Selfine','This for don\'t get self damage'),
(11,5,'Module Happy','Emote module if all modules + bonus'),
(12,5,'Module Evil','Emote module if all modules + bonus'),
(13,5,'Module Surprise','Emote module if all modules + bonus'),
(14,5,'Module Blink','Emote module if all modules + bonus'),
(15,5,'Module Pain','Emote module if all modules + bonus'),
(16,3,'Ticket Create Clan','Ticket for created clan'),
(17,2,'Module Emote','Emote module if all modules + bonus'),
(18,4,'Alpha Box','This bonus in alpha'),
(19,4,'SP Upgrade Box','This bonus in alpha'),
(20,2,'Leather','Craft Item'),
(21,2,'Rare Draw HP Regen','Rare | This drawing item'),
(22,2,'Rare Draw Damage','Rare | This drawing item'),
(23,2,'Rare Draw Handle','Rare | This drawing item'),
(24,2,'Rare Draw Ammo Regen','Rare | This drawing item'),
(25,2,'Rare Ring Slime','Rare | Not self damage and jump'),
(26,5,'Slime Dirt','Item for craft'),
(27,5,'Formula Ring\'s','Item for craft'),
(28,1,'Module Explode Gun','- Weapon / Upgrading'),
(29,1,'Module Explode Shotgun','- Weapon / Upgrading'),
(30,1,'Module Explode Laser','- Weapon / Upgrading'),
(31,1,'Weapon Pressed','- All Weapon +5 ammo'),
(32,5,'Body Boomer','Item for craft'),
(33,2,'Rare Ring Boomer','Rare | Health +5% and item for craft'),
(34,5,'Formula Earring\'s','Item for craft'),
(35,5,'Formula Weapon\'s','Item for craft'),
(36,1,'Module Shotgun Slime','- Weapon / Upgrading'),
(37,1,'Rare Earrings Kwah','Item added +1 jump mabe stack'),
(38,5,'Foot Kwah','Item for craft'),
(39,4,'Book Money x2 (10min)','Book item\'s for added bonus'),
(40,4,'Book Experince x2 (10min)','Book item\'s for added bonus'),
(41,2,'Jioc Dual Ring\'s','Rare | Relation get -10'),
(42,1,'Module Hook Explode','- Module hook damage in explode'),
(43,1,'Auto Hammer','- Auto Fire Hammer'),
(44,1,'Auto Gun','- Auto Fire Gun'),
(45,1,'Module Ghost Gun','- Fire Ghost in Wall'),
(46,1,'Module Ghost Shotgun','- Fire Ghost in Wall'),
(47,1,'Module Ghost Grenade','- Fire Ghost in Wall'),
(48,2,'White Ticket','- Need for Enter in White'),
(49,1,'Rare Freeazer','- %1 freeze for 1sec'),
(50,4,'Rand Box Craft','- This box gived rand craft item'),
(51,4,'Event Box','Event Box'),
(52,1,'Rare Event Hammer','- Snap & added +5 crit damage'),
(53,1,'Bounce Gun','- Gun Ammo Bounce'),
(54,1,'Hybrid SGUN','- Hybrid Gun & Shotgun'),
(55,1,'Bounce Grenade','- Grenade Ammo Bounce'),
(56,4,'Moon O2 / H2O','- This need for area moon'),
(57,1,'Lamp Hammer','- Content 18+'),
(58,3,'Zombie Brain','- Quest Item'),
(59,5,'Zombie Eye','- Craft Item'),
(60,5,'Big Zombie Eye','- Craft Item'),
(61,5,'Skelet Bone','- Craft Item'),
(62,5,'Skelet Fortified Bone','- Craft Item'),
(63,3,'Skelet Skull','- Quest Item'),
(64,5,'Nimf Hearth','- Craft Item'),
(65,5,'Nimf Ears','- Craft Item'),
(66,3,'Nimf Body','- Quest Item'),
(67,4,'Clan Exp Bag','- Added for clan 50000 exp'),
(68,2,'Customized','- This item for custom skin'),
(69,4,'Reseting Upgrades','- This item used'),
(70,4,'Reseting Passive Skills','- Reseting only pasive skill'),
(71,1,'The end Explosion','- This item added end Explosion'),
(72,10,'Output to the chat','- Settings'),
(73,10,'Pick Item','- Settings'),
(74,10,'Skill Walls','- Settings'),
(75,10,'Skill Heal','- Settings'),
(76,4,'VIP PACKAGE','- Added more Bonuses'),
(77,10,'AntiPVP','- Settings'),
(78,2,'VIP Draw Snap','- VIP Item'),
(79,12,'Title VIP','- VIP x2 Money & Exp'),
(80,10,'Skill Sword','- Settings'),
(81,12,'Title Boss?Die','- Spawn HP +1000'),
(82,12,'Title Pig Pig','- Price -5%'),
(83,12,'Title Big Craft','- Not Bonuses'),
(84,10,'Skill Summer Healing','- Event Skill Healing'),
(85,5,'Sun Ray','- Event Craft Item'),
(86,12,'Title Summer','- Title Special Events'),
(87,4,'Potato','- You got 50 exp'),
(88,4,'Tomate','- You got 30 exp'),
(89,4,'Carrot','- You got 20 exp'),
(90,2,'Jump Impuls','- Impuls jump'),
(91,10,'Farming Exp','- Farming Exp'),
(92,4,'Farming Box','- Random Reward'),
(93,12,'Title Questing System','- 1500 HP | 500 Armor'),
(94,5,'Wood','- Craft Item'),
(95,5,'Cooper Ore','- Craft Item'),
(96,6,'Cooper Pickaxe','- Miner Item'),
(97,5,'Iron Ore','- Craft Item'),
(98,6,'Iron Pickaxe','- Miner Item'),
(99,5,'Gold Ore','- Craft Item'),
(100,6,'Gold Pickaxe','- Miner Item'),
(101,5,'Diamond Ore','- Craft Item'),
(102,6,'Diamond Pickaxe','- Miner Item'),
(103,10,'Miner Exp','- Miner Exp'),
(104,5,'Big Wood','- Craft Item'),
(105,5,'Dragon Ore','- Craft Item'),
(106,3,'Particle Soul','- Event Back to school'),
(107,6,'Material','- Work Item'),
(108,10,'Loader Exp','- Loader Exp'),
(109,1,'Pizdamet','- 50% -1 mana'),
(110,15,'Leather body','(Armor +100, Health +100)'),
(111,16,'Leather feet','- Health +50 Armor +50'),
(112,17,'Stabilized Clasic','- Damaage +1'),
(113,15,'Cooper body','- Health Armor +150'),
(114,16,'Cooper feet','- Health Armor +100'),
(115,15,'Iron body','- Health Armor +200'),
(116,16,'Iron feet','- Health Armor +150'),
(117,15,'Gold body','- Health Armor +250'),
(118,16,'Gold feet','- Health Armor +150'),
(119,15,'Diamond body','- Health Armor +300'),
(120,16,'Diamond feet','- Health Armor +250'),
(121,15,'Dragon body','- Health Armor +500'),
(122,16,'Dragon feet','- Health Armor +400'),
(123,10,'Title Enchanter','- Health Armor +1000'),
(124,4,'BOSS SLIME BOX','- Drops from boss Slime'),
(125,2,'SLIME SPHERE','- Gives 10% hp and 5 % armor'),
(126,2,'SLIME NECKLACKE','- Gives 10% hp'),
(127,5,'Pressed piece','- Need for craft ammo+5'),
(128,4,'BOSS VAMPIRE BOX','- Drops from boss Vampire'),
(129,2,'Vampire Fang','- Gives lifesteal'),
(130,4,'DARK KING BOX','-Drops from boss Dark King'),
(131,10,'PIG XP','-Count Pig kills'),
(132,10,'KWAH XP','-Count Kwah kills'),
(133,10,'BOOM XP','-Count Boomer kills'),
(134,10,'PIGKILLER_1','-Achieve'),
(135,10,'PIGKILLER_2','-Achieve'),
(136,10,'PIGKILLER_3','-Achieve'),
(137,10,'PIGKILLER_4','-Achieve'),
(138,10,'PIGKILLER_5','-Achieve'),
(139,10,'KWAHKILLER_1','-Achieve'),
(140,10,'KWAHKILLER_2','-Achieve'),
(141,10,'KWAHKILLER_3','-Achieve'),
(142,10,'KWAHKILLER_4','-Achieve'),
(143,10,'KWAHKILLER_5','-Achieve'),
(144,10,'BOOMKILLER_1','-Achieve'),
(145,10,'BOOMKILLER_2','-Achieve'),
(146,10,'BOOMKILLER_3','-Achieve'),
(147,10,'BOOMKILLER_4','-Achieve'),
(148,10,'BOOMKILLER_5','-Achieve'),
(149,10,'CRAFTMASTER_1','-Achieve'),
(150,10,'CRAFTMASTER_2','-Achieve'),
(151,10,'CRAFTMASTER_3','-Achieve'),
(152,10,'CRAFTMASTER_4','-Achieve'),
(153,10,'CRAFTMASTER_5','-Achieve'),
(154,10,'CRAFTMASTER_6','-Achieve'),
(155,10,'CRAFTMASTER_7','-Achieve'),
(156,15,'Mithril Chestplate','- Health Armor +700'),
(157,16,'Mithril Boots','- Health Armor +600'),
(158,5,'Mithril Ore','- Craft Item'),
(159,15,'Orihalcum Chestplate','- Health Armor +1000'),
(160,16,'Orihalcum Boots','- Health Armor +900'),
(161,5,'Orihalcum Ore','- Craft Item'),
(162,15,'Titanium Chestplate','- Health Armor +1300'),
(163,16,'Titanium Boots','- Health Armor +1200'),
(164,5,'Titanium Ore','- Craft Item'),
(165,15,'Astralium Chestplate','- Health Armor +1600'),
(166,16,'Astralium Boots','- Health Armor +1500'),
(167,5,'Astralium Ore','- Craft Item'),
(168,5,'Slime Soul','- Craft Item'),
(169,10,'CRAFT EXP','- Achieve'),
(170,0,'Fishing Exp','- Fishing exp'),
(171,4,'Common Fish','- You got 10 exp'),
(172,4,'UnCommon Fish','- You got 15 exp'),
(173,4,'Rare Fish','- You got 20 exp'),
(174,4,'Epic Fish','- You got 30 exp'),
(175,4,'Legendary Fish','- You got 45 exp'),
(176,18,'Ice Fish Pet','Pet...'),
(177,18,'Piggy Pet','Pet...'),
(178,18,'Frog Pet','Pet...'),
(179,18,'Bomb Pet','Pet...'),
(180,18,'Dolphin Pet','Pet...'),
(181,18,'Igor Pet','Pet...'),
(182,18,'Tiger Pet','Pet...'),
(183,18,'Monkey Pet','Pet...'),
(184,18,'Mithril Golem Pet','Pet...'),
(185,18,'Cleaver Pet','Pet...'),
(186,4,'Ice Fish','- You got 35 exp'),
(187,4,'Premium Packet','- Thx for premium :D'),
(188,12,'Premium Title','- Thx for premium :D'),
(189,5,'Pixi Dust','- Craft Item'),
(190,19,'Pex Wings','- Wings'),
(191,1,'Storm Laser','- Weapon / Upgrading'),
(192,1,'Target Grenade','- Weapon / Upgrading')
(193,2,'Add Speed Accssory','- Accessory')
(194,2,'Add Damage Accssory','- Accessory')
(195,2,'Add Steal HP Accssory','- Accessory')
(196,2,'Add HP Accssory','- Accessory')
(197,2,'Add Armor Accssory','- Accessory')
(198,2,'Add Money Accssory','- Accessory');
--(116,16,'Iron feet','- Health Armor +150'),(117,15,'Gold body','- Health Armor +250'),(118,16,'Gold feet','- Health Armor +150'),(119,15,'Diamond body','- Health Armor +300'),(120,16,'Diamond feet','- Health Armor +250'),(121,15,'Dragon body','- Health Armor +500'),(122,16,'Dragon feet','- Health Armor +400'),(123,10,'Title Enchanter','- Health Armor +1000'),(124,4,'BOSS SLIME BOX','- Drops from boss Slime'),(125,2,'SLIME SPHERE','- Gives 10% hp and 5 % armor'),(126,2,'SLIME NECKLACKE','- Gives 10% hp'),(127,5,'Pressed piece','- Need for craft ammo+5'),(128,4,'BOSS VAMPIRE BOX','- Drops from boss Vampire'),(129,2,'Vampire Fang','- Gives lifesteal'),(130,4,'DARK KING BOX','-Drops from boss Dark King'),(131,10,'PIG XP','-Count Pig kills'),(132,10,'KWAH XP','-Count Kwah kills'),(133,10,'BOOM XP','-Count Boomer kills'),(134,10,'PIGKILLER_1','-Achieve'),(135,10,'PIGKILLER_2','-Achieve'),(136,10,'PIGKILLER_3','-Achieve'),(137,10,'PIGKILLER_4','-Achieve'),(138,10,'PIGKILLER_5','-Achieve'),(139,10,'KWAHKILLER_1','-Achieve'),(140,10,'KWAHKILLER_2','-Achieve'),(141,10,'KWAHKILLER_3','-Achieve'),(142,10,'KWAHKILLER_4','-Achieve'),(143,10,'KWAHKILLER_5','-Achieve'),(144,10,'BOOMKILLER_1','-Achieve'),(145,10,'BOOMKILLER_2','-Achieve'),(146,10,'BOOMKILLER_3','-Achieve'),(147,10,'BOOMKILLER_4','-Achieve'),(148,10,'BOOMKILLER_5','-Achieve'),(149,10,'CRAFTMASTER_1','-Achieve'),(150,10,'CRAFTMASTER_2','-Achieve'),(151,10,'CRAFTMASTER_3','-Achieve'),(152,10,'CRAFTMASTER_4','-Achieve'),(153,10,'CRAFTMASTER_5','-Achieve'),(154,10,'CRAFTMASTER_6','-Achieve'),(155,10,'CRAFTMASTER_7','-Achieve'),(156,15,'Mithril Chestplate','- Health Armor +700'),(157,16,'Mithril Boots','- Health Armor +600'),(158,5,'Mithril Ore','- Craft Item'),(159,15,'Orihalcum Chestplate','- Health Armor +1000'),(160,16,'Orihalcum Boots','- Health Armor +900'),(161,5,'Orihalcum Ore','- Craft Item'),(162,15,'Titanium Chestplate','- Health Armor +1300'),(163,16,'Titanium Boots','- Health Armor +1200'),(164,5,'Titanium Ore','- Craft Item'),(165,15,'Astralium Chestplate','- Health Armor +1600'),(166,16,'Astralium Boots','- Health Armor +1500'),(167,5,'Astralium Ore','- Craft Item'),(168,5,'Slime Soul','- Craft Item'),(169,10,'CRAFT EXP','- Achieve'),(170,0,'Fishing Exp','- Fishing exp'),(171,4,'Common Fish','- You got 10 exp'),(172,4,'UnCommon Fish','- You got 15 exp'),(173,4,'Rare Fish','- You got 20 exp'),(174,4,'Epic Fish','- You got 30 exp'),(175,4,'Legendary Fish','- You got 45 exp'),(176,18,'Ice Fish Pet','Pet...'),(177,18,'Piggy Pet','Pet...'),(178,18,'Frog Pet','Pet...'),(179,18,'Bomb Pet','Pet...'),(180,18,'Dolphin Pet','Pet...'),(181,18,'Igor Pet','Pet...'),(182,18,'Tiger Pet','Pet...'),(183,18,'Monkey Pet','Pet...'),(184,18,'Mithril Golem Pet','Pet...'),(185,18,'Cleaver Pet','Pet...'),(186,4,'Ice Fish','- You got 35 exp'),(187,4,'Premium Packet','- Thx for premium :D'),(188,12,'Premium Title','- Thx for premium :D');
/*!40000 ALTER TABLE `tw_uitemlist` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `tw_uitems`
--

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
