#include "PlayerController.h"
#include <Time/Time.h>
#include <Classes/Scene/Scene.h>
#include <Runtime/Renderer/Renderer.h>
#include "TurnGameState.h"
#include "../Games.h"

APlayerController::APlayerController() {}

APlayerController::~APlayerController() {}

void APlayerController::Initialize()
{
    // 개척자 소환
    //auto Settler = CreateDefaultSubobject<APlayerCharacter>(L"Setteler" + std::to_wstring(Time::GetElapsedTime()));
    ////auto Settler = std::make_shared<AUnit>(EUnitType::Settler, this);
    //Settler->SetActorLocation(FVector2(2, 2));
    //Units.push_back(Settler);
    int type = static_cast<int>(EUnitType::Settler);
    auto Unit = OwnerScene->NewObject<APlayerCharacter>(APlayerCharacter::GetUnitTypeString(type) + std::to_wstring(Time::GetElapsedTime()), ESCENELAYER::CHARACTER);
    Unit->SetName(APlayerCharacter::GetUnitTypeString(type).c_str());
    Unit->SetUnitType(type);
    Unit->Initialize();
    if (g_TurnGameStateInstanceIsValid)
    {
        Unit->SetActorLocation(g_TurnGameStateInstance->GetMainCamera().get()->GetActorLocation() + Renderer::GetResolution() / 2);
    }
    Units.push_back(Unit);
}

void APlayerController::UpdateTurn()
{
    // 도시 업데이트 (유닛 생산)
    //for (auto& city : Cities)
    //{
    //    city->TurnsToProduce--;
    //    if (city->TurnsToProduce <= 0)
    //    {
    //        auto unit = std::make_shared<AUnit>(EUnitType::Warrior, this);
    //        unit->SetLocation(city->GetLocation());
    //        Units.push_back(unit);
    //        city->TurnsToProduce = 3;
    //    }
    //}
}
void APlayerController::SelectUnitAtPosition(const FVector2& pos)
{
    //for (auto& unit : Units)
    //{
    //    if (unit->bIsDead)
    //        continue;
    //
    //    if (FVector2::Distance(unit->GetLocation(), pos) < 20.0f)
    //    {
    //        SelectedUnit = unit;
    //        printf("유닛 선택됨!\n");
    //        return;
    //    }
    //}

    SelectedUnit = nullptr;
}

void APlayerController::MoveSelectedUnitTo(const FVector2& pos)
{
    if (SelectedUnit)
    {
        SelectedUnit->MoveTo(pos);
        printf("유닛 이동!\n");
    }
}

void APlayerController::BuildCityWithSelectedSettler()
{
    //if (!SelectedUnit || SelectedUnit->UnitType != EUnitType::Settler)
    //    return;
    //
    //auto tile = GetTileAtPosition(SelectedUnit->GetLocation());
    //if (tile && !tile->HasCity)
    //{
    //    auto city = std::make_shared<ACity>();
    //    city->SetLocation(SelectedUnit->GetLocation());
    //    Cities.push_back(city);
    //
    //    tile->HasCity = true;
    //    SelectedUnit->bIsDead = true;
    //    SelectedUnit = nullptr;
    //
    //    printf("도시 건설 완료!\n");
    //}
    //else
    //{
    //    printf("여기는 도시를 지을 수 없습니다.\n");
    //}
}

void APlayerController::HandleInput()
{
    //if (Input::IsMouseClick())
    //{
    //    FVector2 clickPos = Input::GetMouseWorldPosition();
    //    SelectUnitAtPosition(clickPos);
    //}
    //
    //if (Input::IsKeyPressed(KEY_M))
    //{
    //    FVector2 clickPos = Input::GetMouseWorldPosition();
    //    MoveSelectedUnitTo(clickPos);
    //}
    //
    //if (Input::IsKeyPressed(KEY_B))
    //{
    //    BuildCityWithSelectedSettler();
    //}
}
