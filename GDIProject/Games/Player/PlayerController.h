#pragma once
#include <functional>
#include <memory>
#include <string>
#include <vector>
#include <Classes/Object.h>
#include "../Core/GameCommand.h"
#include "../Tile/City.h"
#include "../Tile/Tile.h"
#include "../Character/PlayerCharacter.h"

class UScene;

class APlayerController : public UObject
{
public:
    APlayerController();
    ~APlayerController();

    void Initialize();
    void InitializeForSlot(EPlayerSlot slot);
    void HandleInput(); // 占쏙옙占쏙옙占쏙옙占? 占쌉뤄옙 占쌨억옙 처占쏙옙
    void MoveSelectedUnitTo(const FVector2& pos);
    void UpdateMovement(float deltaSeconds);
    void Release();
    void SetCommandSink(std::function<void(GameCommand)> sink);

    void SpawnAtIndex(int row, int col, int type, std::wstring name);
    void SpawnAtPosition(FVector2 position, EUnitType unitType = EUnitType::Warrior);
    void SpawnStartingUnits();
    std::shared_ptr<APlayerCharacter> FindUnitByStableId(const std::string& stableId) const;
    std::shared_ptr<APlayerCharacter> FindUnitAtIndex(int row, int col) const;
    bool TryMoveUnitToIndex(std::shared_ptr<APlayerCharacter> unit, int row, int col, bool animate, bool emitCommand = false);
    bool TryAttackUnit(std::shared_ptr<APlayerCharacter> attacker, std::shared_ptr<APlayerCharacter> target, bool emitCommand = false);
    bool HasLivingUnits() const;
    std::vector<std::shared_ptr<APlayerCharacter>> GetLivingUnits() const;
    void ReadyUnitsForNextTurn();
    void SkipRemainingUnits();
    std::weak_ptr<APlayerCharacter> GetUnitRefAtPosition(FVector2 position);
    std::weak_ptr<ATile> GetTileRefAtPosition(FVector2 position);

public:
    std::weak_ptr<UObject> OwnerScene;
    std::vector<std::vector<std::weak_ptr<ATile>>> m_tiles;
    std::weak_ptr<APlayerCharacter> SelectedUnit;
    EPlayerSlot Slot = EPlayerSlot::None;

public:
    std::vector<std::weak_ptr<APlayerCharacter>> Units;
    std::vector<std::weak_ptr<ACity>> Cities;

    // 占싱듸옙 占쏙옙占쏙옙
    bool bIsUnitMoving = false;
    std::vector<FVector2> m_movePathWorld;
    int m_movePathIndex = 0;

private:
    std::function<void(GameCommand)> CommandSink;
    bool bSuppressCommandSink = false;
};
