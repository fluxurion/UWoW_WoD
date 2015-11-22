/*!40101 SET NAMES utf8 */;

/*!40101 SET SQL_MODE=''*/;

/*!40014 SET @OLD_UNIQUE_CHECKS=@@UNIQUE_CHECKS, UNIQUE_CHECKS=0 */;
/*!40101 SET @OLD_SQL_MODE=@@SQL_MODE, SQL_MODE='NO_AUTO_VALUE_ON_ZERO' */;
/*!40111 SET @OLD_SQL_NOTES=@@SQL_NOTES, SQL_NOTES=0 */;
/*Table structure for table `spell_concatenate_aura` */

DROP TABLE IF EXISTS `spell_concatenate_aura`;

CREATE TABLE `spell_concatenate_aura` (
  `spellid` int(11) NOT NULL DEFAULT '0',
  `effectSpell` tinyint(11) NOT NULL DEFAULT '0',
  `auraId` int(11) NOT NULL DEFAULT '0',
  `effectAura` tinyint(11) NOT NULL DEFAULT '0',
  `option` int(11) NOT NULL DEFAULT '1',
  `caster` tinyint(3) NOT NULL DEFAULT '0',
  `target` tinyint(3) NOT NULL DEFAULT '0',
  `comment` varchar(100) NOT NULL DEFAULT '',
  PRIMARY KEY (`spellid`,`effectSpell`,`auraId`,`effectAura`,`option`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

/*Data for the table `spell_concatenate_aura` */

insert  into `spell_concatenate_aura`(`spellid`,`effectSpell`,`auraId`,`effectAura`,`option`,`caster`,`target`,`comment`) values (-164273,0,155228,0,1,0,0,'Одинокий волк - при удалении ауры зануляем и обновляем'),(50385,0,48266,2,1,0,0,'Великая власть льда - пересчет амаунта'),(77223,0,34967,1,1,2,1,'Тотем стихий'),(77223,0,34967,2,1,2,1,'Тотем стихий'),(155783,0,155784,1,1,0,0,'Первобытное упорство'),(155783,1,155784,0,1,0,0,'Первобытное упорство'),(157774,0,88766,0,2,0,0,'88766 - Сверкание'),(164273,0,155228,0,1,0,0,'Одинокий волк'),(164273,0,155228,1,1,0,0,'Одинокий волк');

/*!40101 SET SQL_MODE=@OLD_SQL_MODE */;
/*!40014 SET UNIQUE_CHECKS=@OLD_UNIQUE_CHECKS */;
/*!40111 SET SQL_NOTES=@OLD_SQL_NOTES */;