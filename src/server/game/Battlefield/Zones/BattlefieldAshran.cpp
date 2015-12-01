#include "BattlefieldAshran.h"
#include "SpellAuras.h"

bool BattlefieldAshran::SetupBattlefield()
{
    m_isActive = false;
    m_IsEnabled = false;

    m_TypeId = BATTLEFIELD_ASHRAN;
    m_BattleId = BATTLEFIELD_BATTLEID_ASHRAN;
    m_AreaID = 6941;
    m_MapId = 1191;

    m_MaxPlayer = 160;
    m_MinLevel = 98;

    RegisterZone(m_AreaID);

    _stepEndTimer = 1 * MINUTE * IN_MILLISECONDS;
    _nextBattleTimer = 30 * IN_MILLISECONDS;
    _isBattleInProgress = false;
    _currentPoint = CP_CROSSROADS;

    for (uint8 i = TEAM_ALLIANCE; i < MAX_TEAMS; i++)
    {
        _maxTeamKills[i] = 100;
        _currentTeamKills[i] = 0;
        _isBossKilled[i] = false;
        _bossEvent[i] = false;
    }

    m_Timer = 24 * HOUR * IN_MILLISECONDS;

    for (uint8 i = CP_CROSSROADS; i < CP_MAX_POINTS; i++)
    {
        AshranNodePointData* ws = new AshranNodePointData(this);
        ws->Init(SpawnGameObject(ASHRAN_OBJECT_CAPTURE_POINT, AshranNodePoints[i].Pos), AshranNodePoints[i].Worldstate, AshranNodePoints[i].Type, AshranNodePoints[i].InitState);
        _nodePoints.insert(ws);
    }

    return true;
}

bool BattlefieldAshran::Update(uint32 diff)
{
    if (!_isBattleInProgress)
    {
        if (_nextBattleTimer <= diff)
            StartNewBattle();
        else
            _nextBattleTimer -= diff;
    }

    if (_stepEndTimer <= diff)
    {
    } //    StepEnd(true);
    else
        _stepEndTimer -= diff;

    return Battlefield::Update(diff);
}

void BattlefieldAshran::OnGameObjectCreate(GameObject* go)
{
    if (go->GetEntry() != ASHRAN_OBJECT_CAPTURE_POINT)
        return;

    sLog->outError(LOG_FILTER_GENERAL, "BattlefieldAshran::OnGameObjectCreate _currentPoint: %u, goEntry: %u ", _currentPoint, go->GetEntry());

    for (auto const& v : _nodePoints)
    {
        if (v->m_Type != _currentPoint)
            continue;

        AshranCapturePoint* capturePoint = new AshranCapturePoint(this, GetAttackerTeam());
        capturePoint->SetCapturePointData(go);
        capturePoint->LinkToCapturePoint(v);
        AddCapturePoint(capturePoint);
        break;
    }
}

void BattlefieldAshran::OnPlayerJoinWar(Player* player)
{
    if (!player->isAlive())
        player->ResurrectPlayer(1.0f);

    if (player->GetTeamId() == TEAM_ALLIANCE)
        player->TeleportTo(1191, 3958.11f, -4015.49f, 51.31594f, 4.941533f);
    else
        player->TeleportTo(1191, 5053.72f, -4062.88f, 40.5371f, 4.103513f); // @TODO
}

void BattlefieldAshran::OnPlayerLeaveWar(Player* player)
{
    Battlefield::OnPlayerLeaveWar(player);
}

void BattlefieldAshran::OnPlayerLeaveZone(Player* player)
{ }

void BattlefieldAshran::AddPlayerToResurrectQueue(ObjectGuid npcGuid, ObjectGuid playerGuid)
{ }

void BattlefieldAshran::FillInitialWorldStates(WorldPackets::WorldState::InitWorldStates& packet)
{
    packet.Worldstates.emplace_back(WorldStates::ASHRAN_CURRENT_KILLS_ALLIANCE, _currentTeamKills[TEAM_ALLIANCE]);
    packet.Worldstates.emplace_back(WorldStates::ASHRAN_CURRENT_KILLS_HORDE, _currentTeamKills[TEAM_HORDE]);
    packet.Worldstates.emplace_back(WorldStates::ASHRAN_TIME_TO_NEXT_BATTLE, time(nullptr) + _nextBattleTimer / IN_MILLISECONDS);
    packet.Worldstates.emplace_back(WorldStates::ASHRAN_SHOW_TIME_TO_NEXT_BATTLE, _isBattleInProgress);

    if (_bossEvent[TEAM_HORDE])
    {
        packet.Worldstates.emplace_back(WorldStates::ASHRAN_TIME_TO_KILL_VOLRAT, time(nullptr) + _stepEndTimer / IN_MILLISECONDS);
        packet.Worldstates.emplace_back(WorldStates::ASHRAN_SHOW_TIME_TO_KILL_VOLRAT, _currentPoint == WARLORD_VOLRAT_EVENT && _bossEvent[TEAM_HORDE]);
    }

    if (_bossEvent[TEAM_ALLIANCE])
    {
        packet.Worldstates.emplace_back(WorldStates::ASHRAN_SHOW_TIME_TO_KILL_TREMBLADE, _currentPoint == TREMBLADE_EVENT && _bossEvent[TEAM_ALLIANCE]);
        packet.Worldstates.emplace_back(WorldStates::ASHRAN_COMMANDER_TREMBLADE, _currentPoint == TREMBLADE_EVENT && _bossEvent[TEAM_ALLIANCE]);
    }

    packet.Worldstates.emplace_back(WorldStates::ASHRAN_TREMBLADE_AVANGARD_STATE, _currentPoint == CP_ALLIANCE_TOWER);
    packet.Worldstates.emplace_back(WorldStates::ASHRAN_ADVANCED_GATE, _currentPoint == CP_HORDE_TOWER);

    packet.Worldstates.emplace_back(WorldStates::ASHRAN_DISPLAY_CROSSROADS_GRAVEYARD, 1);
    packet.Worldstates.emplace_back(WorldStates::ASHRAN_HIGH_WARLORD_VOLRAT, _currentPoint == WARLORD_VOLRAT_EVENT && _bossEvent[TEAM_HORDE]);
    packet.Worldstates.emplace_back(WorldStates::ASHRAN_CROSSROAD_UI, _currentPoint == CP_CROSSROADS);
    packet.Worldstates.emplace_back(WorldStates::ASHRAN_HORDE_BASE_ICON, !_isBossKilled[TEAM_HORDE]);
    packet.Worldstates.emplace_back(WorldStates::ASHRAN_ALLIANCE_BASE_ICON, !_isBossKilled[TEAM_ALLIANCE]);
    packet.Worldstates.emplace_back(WorldStates::ASHRAN_CAPTURE_FLAG_UI, _currentPoint < CP_MAX_POINTS && !(_isBossKilled[TEAM_ALLIANCE] && _isBossKilled[TEAM_HORDE]));
    packet.Worldstates.emplace_back(WorldStates::ASHRAN_KILLS_CAP_ALLIANCE, _maxTeamKills[TEAM_ALLIANCE]);
    packet.Worldstates.emplace_back(WorldStates::ASHRAN_KILLS_CAP_HORDE, _maxTeamKills[TEAM_HORDE]);

    for (uint8 i = CP_CROSSROADS; i < CP_MAX_POINTS; i++)
        packet.Worldstates.emplace_back(AshranNodePoints[i].Worldstate, AshranNodePoints[i].InitState);
}

void BattlefieldAshran::HandleKill(Player* killer, Unit* victim)
{
    TeamId teamID = killer->GetTeamId();

    if (killer == victim || m_players[teamID].empty())
        return;

    uint8 amount = victim->GetTypeId() == TYPEID_PLAYER ? 5 : 1;

    _currentTeamKills[teamID] += amount;
    if (_currentTeamKills[teamID] >= _maxTeamKills[teamID])
    {
        _maxTeamKills[teamID] *= 2;
        _currentTeamKills[teamID] = 0;

        uint32 currTeamSize = m_players[teamID].size();
        uint32 otherTeamSize = m_players[GetOtherTeamID(teamID)].size();
        uint32 newMaxKillsForTeam = 0;

        if (_maxTeamKills[teamID] = 1000)
        {
            if (currTeamSize > 300)
                newMaxKillsForTeam = 300;
            else if (currTeamSize / otherTeamSize > 3)
                newMaxKillsForTeam = 600;
            else if (currTeamSize < 100 && otherTeamSize > 150)
                newMaxKillsForTeam = 50;
            else if (currTeamSize / otherTeamSize >= 2)
                newMaxKillsForTeam = 100;
        }

        _maxTeamKills[teamID] = newMaxKillsForTeam;
        SendUpdateWorldState(teamID == TEAM_ALLIANCE ? WorldStates::ASHRAN_KILLS_CAP_ALLIANCE : WorldStates::ASHRAN_KILLS_CAP_HORDE, newMaxKillsForTeam);
    }

    SendUpdateWorldState(teamID == TEAM_ALLIANCE ? WorldStates::ASHRAN_CURRENT_KILLS_ALLIANCE : WorldStates::ASHRAN_CURRENT_KILLS_HORDE, _currentTeamKills[teamID]);
    SendWorldTextToTeam(teamID == TEAM_ALLIANCE ? 86489 : 86490, ObjectGuid::Empty, amount, 0);
}

void BattlefieldAshran::UpdateBattleTimers()
{
    switch (_currentPoint)
    {
        case CP_HORDE_TOWER:
            SendUpdateWorldState(WorldStates::ASHRAN_ADVANCED_GATE, 1);
            SendUpdateWorldState(WorldStates::ASHRAN_CAPTURE_FLAG_UI, 1);
        case CP_CROSSROADS:
        case CP_HORDE_OUTPOST:
        case CP_ALLIANCE_OUTPOST:
            SendUpdateWorldState(WorldStates::ASHRAN_CAPTURE_FLAG_UI, 1);
            break;
        case CP_ALLIANCE_TOWER:
            SendUpdateWorldState(WorldStates::ASHRAN_TREMBLADE_AVANGARD_STATE, 1);
            SendUpdateWorldState(WorldStates::ASHRAN_CAPTURE_FLAG_UI, 1);
            break;
        case TREMBLADE_EVENT:
            SendUpdateWorldState(WorldStates::ASHRAN_CAPTURE_FLAG_UI, 0);
            SendUpdateWorldState(WorldStates::ASHRAN_SHOW_TIME_TO_KILL_TREMBLADE, time(nullptr) + _stepEndTimer / 1000);
            SendUpdateWorldState(WorldStates::ASHRAN_COMMANDER_TREMBLADE, _bossEvent[TEAM_ALLIANCE]);
            break;
        case WARLORD_VOLRAT_EVENT:
            SendUpdateWorldState(WorldStates::ASHRAN_CAPTURE_FLAG_UI, 0);
            SendUpdateWorldState(WorldStates::ASHRAN_SHOW_TIME_TO_KILL_VOLRAT, time(nullptr) + _stepEndTimer / 1000);
            SendUpdateWorldState(WorldStates::ASHRAN_HIGH_WARLORD_VOLRAT, _bossEvent[TEAM_HORDE]);
            break;
        default:
            break;
    }

    SendUpdateWorldState(WorldStates::ASHRAN_TIME_TO_NEXT_BATTLE, time(nullptr) + _nextBattleTimer / 1000);
    SendUpdateWorldState(WorldStates::ASHRAN_SHOW_TIME_TO_NEXT_BATTLE, _isBattleInProgress);
}

void BattlefieldAshran::StartNewBattle()
{
    sLog->outError(LOG_FILTER_GENERAL, "BattlefieldAshran::StartNewBattle() _currentPoint: %u ", _currentPoint);

    _isBattleInProgress = true;

    if (_currentPoint != TREMBLADE_EVENT && _currentPoint != WARLORD_VOLRAT_EVENT)
    {
        AshranNodePointData* ws = new AshranNodePointData(this);
        ws->Init(SpawnGameObject(ASHRAN_OBJECT_CAPTURE_POINT, AshranNodePoints[_currentPoint].Pos), AshranNodePoints[_currentPoint].Worldstate, AshranNodePoints[_currentPoint].Type, AshranNodePoints[_currentPoint].InitState);
        _nodePoints.insert(ws);
    }

    UpdateBattleTimers();
}

void BattlefieldAshran::StepEnd(bool byTimer)
{
    sLog->outError(LOG_FILTER_GENERAL, "BattlefieldAshran::StepEnd byTimer %u", byTimer);

    _nextBattleTimer = 30 * IN_MILLISECONDS;
    _isBattleInProgress = false;

    UpdateBattleTimers();
}

void BattlefieldAshran::UpdateStepData(TeamId teamID, CapturePoints type)
{
    switch (type)
    {
        case CP_CROSSROADS:
            _currentPoint = teamID == TEAM_ALLIANCE ? CP_HORDE_OUTPOST : CP_ALLIANCE_OUTPOST;
            break;
        case CP_HORDE_TOWER:
            _currentPoint = teamID == TEAM_ALLIANCE ? WARLORD_VOLRAT_EVENT : CP_HORDE_OUTPOST;
            break;
        case CP_HORDE_OUTPOST:
            _currentPoint = teamID == TEAM_ALLIANCE ? CP_HORDE_TOWER : CP_CROSSROADS;
            break;
        case CP_ALLIANCE_OUTPOST:
            _currentPoint = teamID == TEAM_ALLIANCE ? CP_CROSSROADS : CP_ALLIANCE_TOWER;
            break;
        case CP_ALLIANCE_TOWER:
            _currentPoint = teamID == TEAM_ALLIANCE ? CP_ALLIANCE_OUTPOST : TREMBLADE_EVENT;
            break;
        default:
            sLog->outError(LOG_FILTER_GENERAL, "BattlefieldAshran::UpdateStepData wrong CapturePoint ID %u", type);
            break;
    }
}

void BattlefieldAshran::CapturePoint(TeamId teamID)
{
    uint32 broadcastText[MAX_TEAMS + 1] = {82379, 82380, 0};
    ChatMsg messageType = CHAT_MSG_RAID_BOSS_EMOTE;

    switch (_currentPoint)
    {
        case CP_CROSSROAD_GRAVEYARD:
            broadcastText[teamID] = teamID == TEAM_ALLIANCE ? 85536 : (teamID == TEAM_HORDE ? 85535 : 85537);
            messageType = CHAT_MSG_MONSTER_YELL;
            break;
        case WARLORD_VOLRAT_EVENT:
            broadcastText[teamID] = teamID == TEAM_ALLIANCE ? 89257 : 89344;
            break;
        case TREMBLADE_EVENT:
            broadcastText[teamID] = teamID == TEAM_ALLIANCE ? 89341 : 89256;
            break;
        default:
            break;
    }

    SendBroadcastTextToAll(broadcastText[teamID], messageType);
}

AshranCapturePoint::AshranCapturePoint(BattlefieldAshran* battlefield, TeamId teamInControl) : BfCapturePoint(battlefield)
{
    m_Bf = battlefield;
    m_team = teamInControl;
    m_capturePoint = nullptr;
}

void AshranCapturePoint::PointCapturedByTeam(TeamId teamID)
{
    sLog->outError(LOG_FILTER_GENERAL, "AshranCapturePoint::PointCapturedByTeam oldTeam: %u", teamID);
    m_capturePoint->m_Ashran->CapturePoint(teamID);
    m_capturePoint->m_Ashran->UpdateStepData(teamID, m_capturePoint->m_Type);
    m_capturePoint->m_Ashran->StepEnd(false);

    m_capturePoint->m_Build->Delete();
}

AshranNodePointData::AshranNodePointData(BattlefieldAshran* bf)
{
    m_Ashran = bf;
    m_Build = nullptr;
    m_Type = CP_HORDE_TOWER;
    m_State = NODE_STATE_DEFAULT;
    m_WorldState = WorldStates::WS_NONE;
    m_TeamControl = TEAM_NONE;
}

void AshranNodePointData::Init(GameObject* building, WorldStates worldstate, CapturePoints type, NodePointState state)
{
    m_Build = building;
    m_WorldState = worldstate;
    m_Type = type;
    m_State = state;
}
