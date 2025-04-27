#pragma once
#include <memory>
#include <vector>
#include <Classes/Object.h>
#include "../Tile/City.h"
#include "../Character/Unit.h"
#include "../Tile/Tile.h"

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

private:
    std::shared_ptr<AUnit> SelectedUnit;

public:
    std::vector<std::shared_ptr<AUnit>> Units;
    std::vector<std::shared_ptr<ACity>> Cities;
};
