#include "PlayerController.h"
#include <Time/Time.h>
#include <Classes/Scene/Scene.h>
#include <Runtime/Renderer/Renderer.h>
#include "TurnGameState.h"
#include "../Games.h"
#include <Input/Input.h>
#include <iostream>
#include "../Scene/PlayScene.h"

APlayerController::APlayerController() {}

APlayerController::~APlayerController() {}

void APlayerController::Initialize()
{
    // 개척자 소환
    //auto Settler = CreateDefaultSubobject<APlayerCharacter>(L"Setteler" + std::to_wstring(Time::GetElapsedTime()));
    ////auto Settler = std::make_shared<AUnit>(EUnitType::Settler, this);
    //Settler->SetActorLocation(FVector2(2, 2));
    //Units.push_back(Settler);
    SpawnAtIndex(3, 2);
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

void APlayerController::SpawnAtIndex(int row, int col)
{
    if (row < 0 || col < 0) return;
    int type = static_cast<int>(EUnitType::Settler);
    auto Unit = OwnerScene->NewObject<APlayerCharacter>(APlayerCharacter::GetUnitTypeString(type) + std::to_wstring(Time::GetElapsedTime()), ESCENELAYER::CHARACTER);
    Unit->SetName(APlayerCharacter::GetUnitTypeString(type).c_str());
    Unit->SetUnitType(type);
    Unit->Initialize();
    FVector2 pos = ATile::GetTilePositionAtIndex(col, row);
    FVector2 size = Unit->GetActorSize();
    Unit->row = row;
    Unit->col = col;
    m_tiles[Unit->col][Unit->row]->unit = Unit;
    Unit->SetActorLocation(pos - size / 2);
    Units.push_back(Unit);
}

void APlayerController::SpawnAtPosition(FVector2 position, EUnitType unitType)
{
    if (position.x < 0 || position.y < 0) return;
    int type = static_cast<int>(unitType);
    auto Unit = OwnerScene->NewObject<APlayerCharacter>(APlayerCharacter::GetUnitTypeString(type) + std::to_wstring(Time::GetElapsedTime()), ESCENELAYER::CHARACTER);
    Unit->SetName(APlayerCharacter::GetUnitTypeString(type).c_str());
    Unit->SetUnitType(type);
    Unit->Initialize();
    FVector2 index = ATile::GetIndexAtPosition(position);
    FVector2 pos = ATile::GetTilePositionAtIndex((int)index.x, (int)index.y);
    FVector2 size = Unit->GetActorSize();
    Unit->row = (int)index.y;
    Unit->col = (int)index.x;
    m_tiles[Unit->col][Unit->row]->unit = Unit;
    std::cout << "Spawn Position : " << pos.x << ' ' << pos.y << '\n';
    Unit->SetActorLocation(pos - size / 2);
    Units.push_back(Unit);
}

std::shared_ptr<APlayerCharacter> APlayerController::GetUnitRefAtPosition(FVector2 position)
{
    FVector2 index = ATile::GetIndexAtPosition(position);

    int x = (int)index.x;
    int y = (int)index.y;
    if (x >= TILE_COL_SIZE || y >= TILE_ROW_SIZE) return nullptr;
    if (m_tiles[x][y]->unit)
        return m_tiles[x][y]->unit;
    else
        return nullptr;
}

std::shared_ptr<ATile> APlayerController::GetTileRefAtPosition(FVector2 position)
{
    FVector2 index = ATile::GetIndexAtPosition(position);

    int x = (int)index.x;
    int y = (int)index.y;
    return m_tiles[x][y];
}
void APlayerController::HandleInput()
{
    if (Input::IsKeyPressed(VK_K))
    {
        std::cout << "전체 삭제!";
        for (const auto& Unit : Units)
        {
            OwnerScene->Destroy(Unit);
        }
    }

    if (Input::IsKeyPressed(VK_RBUTTON))
    {
        FVector2 CameraPosition = Game::GetGameState()->GetMainCamera().get()->GetActorLocation();
        SpawnAtPosition(Input::GetMousePosition()+ CameraPosition, EUnitType::Warrior);
    }

    if (Input::IsKeyPressed(VK_LBUTTON))
    {
        FVector2 CameraPosition = Game::GetGameState()->GetMainCamera().get()->GetActorLocation();
        std::shared_ptr<APlayerCharacter> ref = GetUnitRefAtPosition(Input::GetMousePosition()+ CameraPosition);
        if (ref)
        {
            if (std::shared_ptr<UPlayScene> ps = std::dynamic_pointer_cast<UPlayScene>(OwnerScene))
            {
                ps->m_PlayScene_Widget->m_selectTileName->m_content = L"캐릭터 이름 : " + ref->GetName();
                ps->m_PlayScene_Widget->m_selectTileActionCount->m_content = L"남은 행동 수 : " + std::to_wstring(ref->ActionCount);
            }
        }
        else
        {
            if (std::shared_ptr<UPlayScene> ps = std::dynamic_pointer_cast<UPlayScene>(OwnerScene))
            {
                std::shared_ptr<ATile> t = GetTileRefAtPosition(Input::GetMousePosition());
                ps->m_PlayScene_Widget->m_selectTileName->m_content = L"타일 이름 : " + t->GetTileName();
                ps->m_PlayScene_Widget->m_selectTileActionCount->m_content = L"";
            }
        }
    }
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
