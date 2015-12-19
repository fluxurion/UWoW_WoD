UPDATE `creature_template` SET `gossip_menu_id`=16994 WHERE `entry`=81688;
UPDATE `creature_template` SET `gossip_menu_id`=16995 WHERE `entry`=85783;

DELETE FROM `gossip_menu` WHERE `entry`in (16994, 16995);
INSERT IGNORE INTO `gossip_menu` (`entry`, `text_id`) VALUES
(16994, 24741), -- 81688
(16995, 24692); -- 85783


DELETE FROM `gossip_menu_option` WHERE `menu_id` in (16994, 16995);
INSERT IGNORE INTO `gossip_menu_option` (`menu_id`, `id`, `option_icon`, `option_text`, `box_coded`, `box_money`, `box_text`, option_id, npc_option_npcflag) VALUES
(16994, 0, 28, 'Сможешь получить руду из этого дренорского камня?', 0, 0, '', 22, 1), -- 81688
(16995, 0, 28, 'Ты можешь прорастить эти дренорские семена?', 0, 0, '', 22, 1); -- 85783



REPLACE INTO `garrison_shipment` (`NpcEntry`, `ConteinerGoEntry`, `ShipmentID`, `MaxShipments`, `BuildingTypeID`) VALUES 
-- Horde
('81688', '239237', '177', '14', '1'),  -- mine
('85783', '239238', '178', '14', '2');  -- garden


REPLACE INTO `gossip_menu_option` (`menu_id`, `id`, `option_icon`, `option_text`, `option_id`, `npc_option_npcflag`, `action_menu_id`, `action_poi_id`, `box_coded`, `box_money`, `box_text`) VALUES
(0, 18, 28, 'shipment', 22, 1, 0, 0, 0, 0, '');