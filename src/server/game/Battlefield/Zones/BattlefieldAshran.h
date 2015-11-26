

#ifndef BATTLEFIELD_ASHRAN_
#define BATTLEFIELD_ASHRAN_

#include "../Battlefield.h"
#include "Group.h"
#include "WorldPacket.h"
#include "World.h"
#include "ObjectMgr.h"

class BattlefieldAshran;

class AshranCapturePoint;
struct AshranNodePointData;

typedef std::set<AshranNodePointData*> NodePoints;

enum AshranObjects
{
    ASHRAN_OBJECT_CAPTURE_POINT             = 230876,
    ASHRAN_OBJECT_GRAVE_YARD_CAPTURE_POINT  = 231201,

    ASHRAN_OBJECT_ANCIENT_ARTIFACT          = 233822,

    ASHRAN_OBJECT_ALLIANCE_BANNER_25        = 233517, // crossroads spawn
    ASHRAN_OBJECT_HORDE_BANNER_25           = 233518, // crossroads spawn

    ASHRAN_OBJECT_ALLIANCE_OUTPOST_PORTAL   = 233628,

    //< Mine event
    ASHRAN_OBJECT_IMPROWED_ORE              = 233143,
};

enum AshranSpells
{
    SPELL_STAND_FAST        = 173549,
    SPELL_TOWER_DEFENCE     = 173541,
    SPELL_HOLD_YOUR_GROUND  = 173534,
};

enum CapturePoints
{
    CP_CROSSROADS,
    CP_HORDE_TOWER,
    CP_HORDE_OUTPOST,
    CP_ALLIANCE_OUTPOST,
    CP_ALLIANCE_TOWER,

    CP_MAX_POINTS,

    CP_CROSSROAD_GRAVEYARD,
    TREMBLADE_EVENT,
    WARLORD_VOLRAT_EVENT,
};

enum NodePointState
{
    NODE_STATE_DEFAULT,
    NODE_STATE_HORDE,
    NODE_STATE_ALLIANCE,
    NODE_STATE_HIDE
};

struct AshranNodePoint
{
    WorldStates Worldstate;
    CapturePoints Type;
    NodePointState InitState;
    Position Pos;
};

AshranNodePoint const AshranNodePoints[CP_MAX_POINTS] =
{
    {WorldStates::ASHRAN_CROSSROAD_ICON,        CP_CROSSROADS,       NODE_STATE_DEFAULT,  4533.453f, -4211.446f, 7.112216f, 0.0f},
    {WorldStates::ASHRAN_HORDE_TOWER_ICON,      CP_HORDE_TOWER,      NODE_STATE_HORDE,    4801.476f, -4210.983f, 32.94267f, 0.6574f},
    {WorldStates::ASHRAN_HORDE_OUTPOST_ICON,    CP_HORDE_OUTPOST,    NODE_STATE_HORDE,    4677.771f, -4222.424f, 10.10839f, 0.007228867f},
    {WorldStates::ASHRAN_ALLIANCE_OUTPOST_ICON, CP_ALLIANCE_OUTPOST, NODE_STATE_ALLIANCE, 4353.768f, -4190.806f, 10.09851f, 0.0f},
    {WorldStates::ASHRAN_ALLIANCE_TOWER_ICON,   CP_ALLIANCE_TOWER,   NODE_STATE_ALLIANCE, 4226.708f, -4171.363f, 31.2031f,  0.0f}
};

class AshranCapturePoint : public BfCapturePoint
{
public:
    AshranCapturePoint(BattlefieldAshran* battlefield, TeamId teamInControl);

    void LinkToCapturePoint(AshranNodePointData* point) { m_capturePoint = point; }
    void PointCapturedByTeam(TeamId teamID) override;

    TeamId GetTeam() const { return m_team; }

protected:
    AshranNodePointData* m_capturePoint;
};

class BattlefieldAshran : public Battlefield
{
public:
    struct PointInfo
    {
        uint32 pointID;
        TeamId teamID;
    };

    bool SetupBattlefield() override;
    bool Update(uint32 diff) override;
    void AddPlayerToResurrectQueue(ObjectGuid npcGuid, ObjectGuid playerGuid) override;

    void FillInitialWorldStates(WorldPackets::WorldState::InitWorldStates& packet) override;
    void OnPlayerJoinWar(Player* player) override;
    void OnPlayerLeaveZone(Player* player) override;
    void OnPlayerLeaveWar(Player* player) override;
    void OnGameObjectCreate(GameObject* go) override;

    void UpdateStepData(TeamId teamID, CapturePoints type);

    void HandleKill(Player* killer, Unit* victim) override;
    void StartNewBattle();
    void UpdateBattleTimers();
    void StepEnd(bool byTimer);
    void CapturePoint(TeamId teamID);

private:
    NodePoints _nodePoints;

    uint32 _stepEndTimer;
    uint32 _nextBattleTimer;
    CapturePoints _currentPoint;

    uint32 _maxTeamKills[MAX_TEAMS];
    uint32 _currentTeamKills[MAX_TEAMS];

    bool _bossEvent[MAX_TEAMS];
    bool _isBattleInProgress;
    bool _isBossKilled[MAX_TEAMS];
};

struct AshranNodePointData
{
    AshranNodePointData(BattlefieldAshran* bf);

    void Init(GameObject* building, WorldStates worldstate, CapturePoints type, NodePointState state);

    BattlefieldAshran* m_Ashran;
    GameObject* m_Build;
    CapturePoints m_Type;
    NodePointState m_State;
    WorldStates m_WorldState;
    uint32 m_TeamControl;
};

#endif // BATTLEFIELD_ASHRAN_
