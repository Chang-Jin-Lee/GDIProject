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
		FVector2 unitIndex = ATile::GetIndexAtPosition(SelectedUnit->GetActorLocation());
		if (unitIndex.x == -1 || unitIndex.y == -1) return;
		FVector2 index = ATile::GetIndexAtPosition(pos);
		FVector2 pos = ATile::GetTilePositionAtIndex((int)index.x, (int)index.y);
		if (pos.x == -1 || pos.y == -1) return;

		int x = (int)index.x;
		int y = (int)index.y;
		std::cout << "이동하려는 위치 x,y : " << x << ' ' << y << '\n';

		int distance = ATile::SearchCost(int(unitIndex.x), int(unitIndex.y), x, y, m_tiles);

		if (distance == -1)
		{
			printf("해당 칸으로 이동할 수 없습니다.\n");
			return;
		}
		else
		{
			if (distance <= SelectedUnit->ActionRemainCount)
			{
				m_tiles[(int)unitIndex.x][(int)unitIndex.y]->unit = nullptr;
				SelectedUnit->ActionRemainCount -= distance;
				m_tiles[x][y]->unit = SelectedUnit;
				SelectedUnit->SetActorLocation(pos);
				SelectedUnit = nullptr;
				printf("유닛 이동!\n");
				for (auto& tiles : m_tiles)
				{
					for (auto& tile : tiles)
					{
						tile->SetHighlight(false);
					}
				}
			}
			else
			{
				printf("해당 칸으로 이동할 수 없습니다.\n");
			}

		}
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
	FVector2 index = ATile::GetIndexAtPosition(position);
	FVector2 pos = ATile::GetTilePositionAtIndex((int)index.x, (int)index.y);
	if (pos.x == -1 || pos.y == -1) return;

	auto Unit = OwnerScene->NewObject<APlayerCharacter>(APlayerCharacter::GetUnitTypeString(type) + std::to_wstring(Time::GetElapsedTime()), ESCENELAYER::CHARACTER);
	Unit->SetName(APlayerCharacter::GetUnitTypeString(type).c_str());
	Unit->SetUnitType(type);
	Unit->Initialize();
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

	if (x == -1 || y == -1) return nullptr;

	if (m_tiles[x][y]->unit)
		return m_tiles[x][y]->unit;
	else
		return nullptr;
}

std::shared_ptr<ATile> APlayerController::GetTileRefAtPosition(FVector2 position)
{
	FVector2 index = ATile::GetIndexAtPosition(position);

	if (index.x == -1 || index.y == -1) return nullptr;

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
		//SpawnAtPosition(Input::GetMouseWorldPosition(Game::GetGameState()->GetMainCamera()), EUnitType::Warrior);
		if (SelectedUnit)
		{
			MoveSelectedUnitTo(Input::GetMouseWorldPosition(Game::GetGameState()->GetMainCamera()));
		}
	}

	if (Input::IsKeyPressed(VK_LBUTTON))
	{
		SelectedUnit = GetUnitRefAtPosition(Input::GetMouseWorldPosition(Game::GetGameState()->GetMainCamera()));
		if (SelectedUnit)
		{
			if (std::shared_ptr<UPlayScene> ps = std::dynamic_pointer_cast<UPlayScene>(OwnerScene))
			{
				ps->m_PlayScene_Widget->m_selectTileName->m_content = L"캐릭터 이름 : " + SelectedUnit->GetName();
				ps->m_PlayScene_Widget->m_selectTileActionCount->m_content = L"남은 행동 수 : " + std::to_wstring(SelectedUnit->ActionRemainCount);

				// 미리보기
				FVector2 unitIndex = ATile::GetIndexAtPosition(SelectedUnit->GetActorLocation());
				std::vector<std::pair<int, int>> reachable;
				ATile::GetReachableTiles(int(unitIndex.x), int(unitIndex.y), SelectedUnit->ActionRemainCount, m_tiles, reachable);
				if (reachable.empty())
				{
					printf("해당 칸으로 이동할 수 없습니다.\n");
					for (auto& tiles : m_tiles)
					{
						for (auto& tile : tiles)
						{
							tile->SetHighlight(false);
						}
					}
					return;
				}
				else
				{
					for (auto& tiles : m_tiles)
					{
						for (auto& tile : tiles)
						{
							tile->SetHighlight(false);
						}
					}
					for (auto& tile : reachable)
					{
						int r = tile.first;
						int c = tile.second;
						m_tiles[r][c]->SetHighlight(true); // 미리보기 표시
					}
				}
			}
		}
		else
		{
			for (auto& tiles : m_tiles)
			{
				for (auto& tile : tiles)
				{
					tile->SetHighlight(false);
				}
			}
			if (std::shared_ptr<UPlayScene> ps = std::dynamic_pointer_cast<UPlayScene>(OwnerScene))
			{
				std::shared_ptr<ATile> t = GetTileRefAtPosition(Input::GetMouseWorldPosition(Game::GetGameState()->GetMainCamera()));
				if (t)
				{
					ps->m_PlayScene_Widget->m_selectTileName->m_content = L"타일 이름 : " + t->GetTileName();
					ps->m_PlayScene_Widget->m_selectTileActionCount->m_content = L"";
				}
				else
				{
					ps->m_PlayScene_Widget->m_selectTileName->m_content = L"타일 이름 : ";
					ps->m_PlayScene_Widget->m_selectTileActionCount->m_content = L"";

				}
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
