#pragma once
#include <memory>
#include <vector>
#include <Classes/Object.h>
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
    void HandleInput(); // 사용자의 입력 받아 처리
    void MoveSelectedUnitTo(const FVector2& pos);
    void Release();

    void SpawnAtIndex(int row, int col, int type, std::wstring name);
    void SpawnAtPosition(FVector2 position, EUnitType unitType = EUnitType::Warrior);
    std::weak_ptr<APlayerCharacter> GetUnitRefAtPosition(FVector2 position);
    std::weak_ptr<ATile> GetTileRefAtPosition(FVector2 position);

public:
    std::weak_ptr<UObject> OwnerScene;
    std::vector<std::vector<std::weak_ptr<ATile>>> m_tiles;

private:
    std::weak_ptr<APlayerCharacter> SelectedUnit;

public:
    std::vector<std::weak_ptr<APlayerCharacter>> Units;
    std::vector<std::weak_ptr<ACity>> Cities;
};
