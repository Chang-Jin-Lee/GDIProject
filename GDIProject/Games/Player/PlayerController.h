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
    void UpdateTurn(); // 매 턴마다 호출


    void HandleInput(); // 사용자의 입력 받아 처리
    void SelectUnitAtPosition(const FVector2& pos);
    void MoveSelectedUnitTo(const FVector2& pos);
    void BuildCityWithSelectedSettler();

    void SpawnAtIndex(int row, int col);
    void SpawnAtPosition(FVector2 position, EUnitType unitType = EUnitType::Warrior);
    std::shared_ptr<APlayerCharacter> GetUnitRefAtPosition(FVector2 position);
    std::shared_ptr<ATile> GetTileRefAtPosition(FVector2 position);

public:
    std::shared_ptr<UScene> OwnerScene;
    std::vector<std::vector<std::shared_ptr<ATile>>> m_tiles;

private:
    std::shared_ptr<APlayerCharacter> SelectedUnit;

public:
    std::vector<std::shared_ptr<APlayerCharacter>> Units;
    std::vector<std::shared_ptr<ACity>> Cities;
};
