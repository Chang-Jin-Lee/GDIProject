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

public:
    std::shared_ptr<UScene> OwnerScene;

private:
    std::shared_ptr<APlayerCharacter> SelectedUnit;

public:
    std::vector<std::shared_ptr<APlayerCharacter>> Units;
    std::vector<std::shared_ptr<ACity>> Cities;
};
