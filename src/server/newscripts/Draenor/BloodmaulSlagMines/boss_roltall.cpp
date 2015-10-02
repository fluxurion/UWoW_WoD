/*
 * Copyright (C) 2008-2015 TrinityCore <http://www.trinitycore.org/>
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the
 * Free Software Foundation; either version 2 of the License, or (at your
 * option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for
 * more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program. If not, see <http://www.gnu.org/licenses/>.
 */

#include "bloodmaul_slag_mines.h"
#include "MoveSplineInit.h"

enum Spells
{
    SPELL_FIERY_BOULDER_1       = 152741,
    SPELL_FIERY_BOULDER_2       = 152850, //< triggered from SPELL_FIERY_BOULDER_2

    SPELL_FIERY_BOULDER_3       = 153058,
    SPELL_FIERY_BOULDER_4       = 152742,
    SPELL_FIERY_BOULDER_5       = 152835,
    SPELL_FIERY_BOULDER_6       = 152837,
    SPELL_FIERY_BOULDER_7       = 152843,
    SPELL_ALPHA_FADE_OUT        = 141608,

    SPELL_FIERY_BOULDER         = 153247, //< casted by roltall on self
    SPELL_HEAT_WAVE             = 152897,
    SPELL_HEAT_WAVE_2           = 152940,
    SPELL_BURNING_SLAG          = 153227,
    SPELL_BURNING_SLAG_2        = 152939,
    SPELL_SCORCHING_AURA        = 167739, //< Heroic
    SPELL_SCORCHING_AURA_AT     = 167738,
};

enum NPCs
{
    NPC_HEAT_WAVE               = 75865, //< casting SPELL_HEAT_WAVE

    NPC_FIERY_BOULDER_RIGHT     = 75828, //< 1
    NPC_FIERY_BOULDER_LEFT      = 75853, //< 2
    NPC_FIERY_BOULDER_MIDDLE    = 75854, //< 3
};

Position const rightBoulder[] =   //< NPC_FIERY_BOULDER_RIGHT
{
    {2297.642f, -204.163f, 253.099f},   // стартовая позиция Falling (64) 
    {2297.642f, -204.163f, 212.631f},   // позиция 2 - упали в низ, как только достигаем позиции вешаем Flags: Flying (512) 
    {2296.247f, -204.237f, 213.545f},   // [0] WayPoints точка старта от босса
    {2249.747f, -205.987f, 213.545f}    // [1] WayPoints точка на другой стороне моста

    /*

    Кастуем сами на себя спелл SPELL_FIERY_BOULDER_1 и получаем триггером SPELL_FIERY_BOULDER_2

    Стартовая позиция:
    Position:       2297.642 Y: -204.163, 253.099     Flags: Falling (64)

    прямо от неё мы двигаемся сюда
    Position:       2297.642 Y: -204.163, 212.631

    через 2 секунды кастуем на себя SPELL_FIERY_BOULDER_3 и получаем триггером SPELL_FIERY_BOULDER_4

    и сразу начинаем двигаеться от позиции
    Position:       2297.642 Y: -204.163, 212.631    Flags: Flying (512)

    по позициям
    [0] Points:     2208.351 Y: -208.3108,213.9586

    [0] WayPoints:  2296.247 Y: -204.237, 213.545
    [1] WayPoints:  2249.747 Y: -205.987, 213.545

    далее кастуем на себя спелл SPELL_ALPHA_FADE_OUT

    */
};

Position const leftBoulder[] =    //< NPC_FIERY_BOULDER_LEFT
{
    {2298.648f, -218.293f, 253.099f},   // стартовая позиция Falling (64) 
    {2298.648f, -218.293f, 212.631f},   // позиция 2 - упали в низ, как только достигаем позиции вешаем Flags: Flying (512) 
    {2294.631f, -218.836f, 213.221f},   // [0] WayPoints точка старта от босса
    {2251.131f, -220.086f, 213.471f}    // [1] WayPoints точка на другой стороне моста

    /*

    Кастуем сами на себя спелл SPELL_FIERY_BOULDER_1 и получаем триггером SPELL_FIERY_BOULDER_2

    Стартовая позиция:
    Position:       2298.648 Y: -218.293, 253.099      Flags: Falling (64)

    прямо от неё мы двигаемся сюда
    Points:         2298.648 Y: -218.293, 212.631

    через 2 секунды кастуем на себя SPELL_FIERY_BOULDER_3 и получаем триггером SPELL_FIERY_BOULDER_5

    и сразу начинаем двигаеться от позиции
    Position:       2298.648 Y: -218.293, 212.631     Flags: Flying (512)
    по позициям
    [0] Points:     2209.615 Y: -221.880, 214.312

    [0] WayPoints:  2294.631 Y: -218.836, 213.2216
    [1] WayPoints:  2251.131 Y: -220.086, 213.4716

    далее кастуем на себя спелл SPELL_ALPHA_FADE_OUT

    */
};

Position const middleBoulder[] =  //< NPC_FIERY_BOULDER_MIDDLE
{
    {2297.642f, -211.868f, 253.099f},   // стартовая позиция Falling (64) 
    {2297.642f, -211.868f, 212.667f},   // позиция 2 - упали в низ, как только достигаем позиции вешаем Flags: Flying (512), после достижения позиции идёт каст SPELL_FIERY_BOULDER_3
    {2295.813f, -212.111f, 213.075f},   // [0] WayPoints точка старта от босса
    {2247.813f, -212.111f, 213.575f},   // [1] WayPoints точка на другой стороне моста
    {2212.783f, -212.335f, 213.937f}    // точка перед отправкой назад

    /*

    Кастуем сами на себя спелл SPELL_FIERY_BOULDER_1 и получаем триггером SPELL_FIERY_BOULDER_2

    Стартовая позиция:
    Position:       2297.642, -211.868, 253.099     Flags: Falling (64)

    прямо от неё мы двигаемся сюда
    [0] Points:     2297.642, -211.8681, 212.667

    через 2 секунды кастуем на себя SPELL_FIERY_BOULDER_3 и получаем триггером SPELL_FIERY_BOULDER_6

    и сразу начинаем двигаеться от позиции
    Position:       2297.642, -211.868, 212.667    Flags: Flying (512)

    по позициям
    [0] Points:     2209.984, -212.354, 213.983

    [0] WayPoints:  2295.813, -212.111, 213.075
    [1] WayPoints:  2247.813, -212.111, 213.575

    далее кастуем на себя спелл SPELL_FIERY_BOULDER_7

    далее в позиции
    Position:       2212.783, -212.3358, 213.937    Flags: None (0)

    и после этого кастуем на себя  SPELL_ALPHA_FADE_OUT

    */
};

class boss_roltall : public CreatureScript
{
public:
    boss_roltall() : CreatureScript("boss_roltall") { }

    struct boss_roltallAI : public BossAI
    {
        boss_roltallAI(Creature* creature) : BossAI(creature, DATA_ROLTALL)
        {
            instance = me->GetInstanceScript();

            me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_DISABLE_MOVE);
            SetCombatMovement(false);
        }

        void Reset()
        {
            _Reset();
        }

        void EnterCombat(Unit* /*who*/)
        {
            _EnterCombat();

            events.ScheduleEvent(ACTION_4, 5 * IN_MILLISECONDS);
            if (IsHeroic())
                events.ScheduleEvent(ACTION_3, 1 * IN_MILLISECONDS);
        }

        void JustDied(Unit* /*killer*/)
        {
            _JustDied();

            if (IsHeroic())
                instance->DoRemoveAurasDueToSpellOnPlayers(SPELL_SCORCHING_AURA);
        }

        void EnterEvadeMode()
        {
            _EnterEvadeMode();

            if (IsHeroic())
                instance->DoRemoveAurasDueToSpellOnPlayers(SPELL_SCORCHING_AURA);
        }

        void UpdateAI(uint32 diff)
        {
            if (!UpdateVictim())
                return;

            events.Update(diff);

            if (me->HasUnitState(UNIT_STATE_CASTING))
                return;

            while (uint32 eventId = events.ExecuteEvent())
            {
                switch (eventId)
                {
                    case ACTION_1:
                        break;
                    case ACTION_2:
                        break;
                    case ACTION_3:
                        me->AddAura(SPELL_SCORCHING_AURA_AT, me);
                        break;
                    default:
                        break;
                }
            }

            DoMeleeAttackIfReady();
        }

    private:
        InstanceScript* instance;
        EventMap events;
    };

    CreatureAI* GetAI(Creature* creature) const
    {
        return new boss_roltallAI(creature);
    }
};

void AddSC_boss_roltall()
{
    new boss_roltall();
}
