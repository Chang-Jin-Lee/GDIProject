#pragma once
#include <memory>
#include <vector>
#include "../Tile/City.h"
#include "../Tile/Tile.h"
#include "../Character/PlayerCharacter.h"

class UScene;

class AAIPlayer : public UObject
{
public:
    AAIPlayer();
    ~AAIPlayer();

    void Initialize();
    void UpdateTurn(); // 턴마다 호출

private:
    void UpdateUnits();
    void UpdateCities();
    void HandleSettler(std::shared_ptr<APlayerCharacter> Settler);
    void HandleCombatUnit(std::shared_ptr<APlayerCharacter> CombatUnit);

    FVector2 FindNearbyGoodTile(const FVector2& currentPos);
    std::shared_ptr<APlayerCharacter> FindNearestEnemy(const FVector2& currentPos);

    int EvaluateTile(std::shared_ptr<ATile> tile);

    int GetTileX(const FVector2& pos) { return static_cast<int>(pos.x / 98); }
    int GetTileY(const FVector2& pos) { return static_cast<int>(pos.y / 120); }

public:
    std::shared_ptr<UScene> OwnerScene;
    std::vector<std::vector<std::shared_ptr<ATile>>> m_tiles;

public:
    std::vector<std::shared_ptr<APlayerCharacter>> Units;
    std::vector<std::shared_ptr<ACity>> Cities;
};
