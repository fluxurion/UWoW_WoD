/*
SQLyog Community v12.14 (64 bit)
MySQL - 5.6.24-log : Database - trinworld542
*********************************************************************
*/

/*!40101 SET NAMES utf8 */;

/*!40101 SET SQL_MODE=''*/;

/*!40014 SET @OLD_UNIQUE_CHECKS=@@UNIQUE_CHECKS, UNIQUE_CHECKS=0 */;
/*!40101 SET @OLD_SQL_MODE=@@SQL_MODE, SQL_MODE='NO_AUTO_VALUE_ON_ZERO' */;
/*!40111 SET @OLD_SQL_NOTES=@@SQL_NOTES, SQL_NOTES=0 */;
/*Table structure for table `package_antispamm` */

DROP TABLE IF EXISTS `package_antispamm`;

CREATE TABLE `package_antispamm` (
  `id` int(11) NOT NULL,
  `maxcount` int(11) DEFAULT '0',
  `second` int(11) DEFAULT '0',
  `comment` varchar(50) DEFAULT '',
  PRIMARY KEY (`id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

/*Data for the table `package_antispamm` */

insert  into `package_antispamm`(`id`,`maxcount`,`second`,`comment`) values (458,1,3,'CMSG_REQUEST_STABLED_PETS'),(576,1,5,'CMSG_SEND_MAIL'),(669,1,1,'CMSG_DF_LEAVE'),(693,1,3,'CMSG_CALENDAR_EVENT_INVITE'),(926,1,3,'CMSG_ADD_FRIEND'),(1372,10,1,'CMSG_MOVE_HEARTBEAT'),(1558,1,3,'CMSG_CHARACTER_RENAME_REQUEST'),(1744,3,1,'CMSG_USE_ITEM'),(1814,2,5,'CMSG_GUILD_INVITE_BY_NAME'),(1950,1,30,'CMSG_WHO'),(2108,3,3,'CMSG_USE_EQUIPMENT_SET'),(2529,1,3,'CMSG_PET_ABANDON'),(2590,1,5,'CMSG_GM_TICKET_CREATE'),(2743,1,5,'CMSG_CHANGE_SUB_GROUP'),(3505,1,5,'CMSG_RECLAIM_CORPSE'),(3736,1,5,'CMSG_PLAYER_LOGIN'),(4250,1,5,'CMSG_CHAT_CHANNEL_UNMUTE'),(4270,1,5,'CMSG_CHAT_CHANNEL_UNMODERATOR'),(4537,1,5,'CMSG_CHAT_CHANNEL_ANNOUNCEMENTS'),(4663,2,1,'CMSG_DF_JOIN'),(4797,40,20,'CMSG_PARTY_INVITE'),(4859,3,1,'CMSG_CANCEL_AURA'),(5165,1,5,'CMSG_CHAT_CHANNEL_INVITE'),(5322,1,5,'CMSG_CHAT_CHANNEL_SET_OWNER'),(5421,1,5,'CMSG_CHAT_CHANNEL_MODERATOR'),(5437,1,5,'CMSG_CHAT_CHANNEL_KICK'),(5469,1,5,'CMSG_CHAT_CHANNEL_UNBAN'),(5613,1,5,'CMSG_CHAT_CHANNEL_OWNER'),(5686,1,3,'CMSG_CREATE_CHARACTER'),(5792,1,3,'CMSG_ADD_IGNORE'),(6270,1,5,'CMSG_CHAT_CHANNEL_BAN'),(6409,1,5,'CMSG_CHAT_CHANNEL_MUTE'),(6449,15,1,'CMSG_SELL_ITEM'),(6458,1,5,'CMSG_CHAT_CHANNEL_PASSWORD'),(6914,1,1,'CMSG_SET_PET_SLOT'),(7083,1,3,'CMSG_TOGGLE_PVP'),(7602,1,2,'CMSG_SPELL_CLICK');

/*!40101 SET SQL_MODE=@OLD_SQL_MODE */;
/*!40014 SET UNIQUE_CHECKS=@OLD_UNIQUE_CHECKS */;
/*!40111 SET SQL_NOTES=@OLD_SQL_NOTES */;
