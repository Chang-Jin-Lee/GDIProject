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
    //bool IsValidTile(int x, int y) { return (x >= 0 && y >= 0 && x < TILE_COL_SIZE && y < TILE_ROW_SIZE); }

public:
    std::shared_ptr<UScene> OwnerScene;

public:
    std::vector<std::shared_ptr<APlayerCharacter>> Units;
    std::vector<std::shared_ptr<ACity>> Cities;
};
