#include "PlayerController.h"
#include <Time/Time.h>
#include <Classes/Scene/Scene.h>
#include <Runtime/Renderer/Renderer.h>
#include "TurnGameState.h"
#include "../Games.h"
#include <Input/Input.h>
#include <iostream>
#include "../Scene/PlayScene.h"
#include <UI/UITextComponent.h>
#include <UI/UIButtonComponent.h>

APlayerController::APlayerController() 
{

}

APlayerController::~APlayerController() 
{
	OwnerScene.reset(); 
	for (auto& m_tile : m_tiles)
	{
		for (auto& tile : m_tile)
		{
			tile.reset();
		}
	}
	SelectedUnit.reset();
	for (auto& Unit : Units)
	{
		Unit.reset();
	}
	for (auto& Citie : Cities)
	{
		Citie.reset();
	}
}

void APlayerController::Initialize()
{
	// 개척자 소환
	//SpawnAtIndex(3, 2);
	//SpawnAtIndex(3, 2);
	//SpawnAtIndex(3, 2);

	std::vector<std::pair<int, int>> indexVector;
	for (int i = 0; i < 3; i++)
	{
		int x = (int)FRandom::GetRandomInRange(0, TILE_ROW_SIZE);
		int y = (int)FRandom::GetRandomInRange(0, TILE_COL_SIZE * 0.2);
		while (std::find(indexVector.begin(), indexVector.end(), std::make_pair(x, y)) != indexVector.end())
		{
			x = (int)FRandom::GetRandomInRange(0, TILE_ROW_SIZE);
			y = (int)FRandom::GetRandomInRange(0, TILE_COL_SIZE * 0.2);
		}
		indexVector.emplace_back(x, y);
		SpawnAtIndex(y, x, i, std::to_wstring(y)+L"_"+std::to_wstring(x));
	}
	std::vector<std::pair<int, int>>().swap(indexVector);
}

void APlayerController::MoveSelectedUnitTo(const FVector2& pos)
{
	if (const auto& SelectedUnitRef = SelectedUnit.lock())
	{
		if (bIsUnitMoving) return; // 이동 중에는 새로운 명령 무시
    	// 현재 유닛의 타일 인덱스는 유닛 중심 기준으로 계산하여 오차 방지
    	FVector2 unitCenter = SelectedUnitRef->GetActorLocation() + SelectedUnitRef->GetActorSize() / 2;
    	FVector2 unitIndex = ATile::GetIndexAtPosition(unitCenter);
		if (unitIndex.x == -1 || unitIndex.y == -1) return;
		FVector2 index = ATile::GetIndexAtPosition(pos);
		FVector2 pos = ATile::GetTilePositionAtIndex((int)index.x, (int)index.y);
		if (pos.x == -1 || pos.y == -1) return;
   		 // 동일 타일이면 이동 금지
    	if ((int)unitIndex.x == (int)index.x && (int)unitIndex.y == (int)index.y) return;

		int x = (int)index.x;
		int y = (int)index.y;
    	int distance = ATile::SearchCost(int(unitIndex.x), int(unitIndex.y), x, y, m_tiles);

		if (distance == -1)
		{
			return;
		}
        else
		{
            // 동일 타일이면 이동 금지
            if (distance == 0) return;
            if (distance <= SelectedUnitRef->ActionRemainCount)
			{
				// 경로 구성: 현재 위치에서 목적지까지 비용이 감소하는 이웃을 따라 진행
				m_movePathWorld.clear();
				m_movePathIndex = 0;
				int curRow = (int)unitIndex.x;
				int curCol = (int)unitIndex.y;
				const int targetRow = x;
				const int targetCol = y;
				int guard = TILE_ROW_SIZE * TILE_COL_SIZE; // 안전 가드
				while ((curRow != targetRow || curCol != targetCol) && guard-- > 0)
				{
					const int* dx = (curRow % 2 == 0) ? even_dx : odd_dx;
					const int* dy = (curRow % 2 == 0) ? even_dy : odd_dy;
					int bestRow = curRow, bestCol = curCol;
					int bestCost = INT_MAX;
					for (int d = 0; d < 6; ++d)
					{
						const int nr = curRow + dy[d];
						const int nc = curCol + dx[d];
						if (!ATile::IsValidIndex(nr, nc)) continue;
						if (auto t = m_tiles[nr][nc].lock())
						{
							if (!t->bVisible) continue;
							// 현재 경로 계산 단계에서는 타겟만 예외적으로 허용
							if (t->unit.expired() == false && !(nr == targetRow && nc == targetCol)) continue;
						}
						const int cost = ATile::SearchCost(nr, nc, targetRow, targetCol, m_tiles);
						if (cost != -1 && cost < bestCost)
						{
							bestCost = cost;
							bestRow = nr; bestCol = nc;
						}
					}
					// 더 이상 진행 불가
					if (bestRow == curRow && bestCol == curCol) break;
					curRow = bestRow; curCol = bestCol;
					FVector2 wpCenter = ATile::GetTilePositionAtIndex(curRow, curCol);
					FVector2 halfSize = SelectedUnitRef->GetActorSize() / 2;
					m_movePathWorld.push_back(wpCenter - halfSize);
				}

				// 경로가 유효할 때만 타일 점유/해제 및 이동 시작
				if (!m_movePathWorld.empty())
				{
					// 목적지 타일에 유닛 점유 예약
					if (const auto tile = m_tiles[x][y].lock()) { tile->unit = SelectedUnit; }
					// 출발 타일 비우기
					if (const auto tile = m_tiles[(int)unitIndex.x][(int)unitIndex.y].lock()) { tile->unit.reset(); }
				}

				// 행동력 차감 및 UI 업데이트
				SelectedUnitRef->ActionRemainCount -= distance;
				if (const auto OwnerSceneRef = OwnerScene.lock())
				{
					std::shared_ptr<UPlayScene> ps = std::dynamic_pointer_cast<UPlayScene>(OwnerSceneRef);
					if (auto widget = ps->m_PlayScene_Widget.lock())
					{
						if (auto text = widget->m_selectTileName.lock())
						{
							text->SetContent(L"캐릭터 이름 : " + SelectedUnitRef->GetName());
						}
						if (auto text2 = widget->m_selectTileActionCount.lock())
						{
							text2->SetContent(L"남은 행동 수 : " + std::to_wstring(SelectedUnitRef->ActionRemainCount));
						}
					}
				}

				// 하이라이트 해제
				for (auto& tiles : m_tiles)
				{
					for (auto& tile : tiles)
					{
						if (const auto tileRef = tile.lock())
						{
							tileRef->SetHighlight(false);
						}
					}
				}

				// 이동 시작
				if (!m_movePathWorld.empty())
				{
					bIsUnitMoving = true;
					m_movePathIndex = 0;
					SelectedUnitRef->animstate = APlayerCharacter::AnimationState::Run;
				}
			}
		}
	}
}

void APlayerController::UpdateMovement(float deltaSeconds)
{
	if (!bIsUnitMoving) return;
	auto unit = SelectedUnit.lock();
	if (!unit) { bIsUnitMoving = false; m_movePathWorld.clear(); return; }
	if (m_movePathIndex >= (int)m_movePathWorld.size()) { bIsUnitMoving = false; m_movePathWorld.clear(); unit->animstate = APlayerCharacter::AnimationState::Idle; return; }

	FVector2 target = m_movePathWorld[m_movePathIndex];
	FVector2 current = unit->GetActorLocation();
	FVector2 diff = target - current;
	const float len2 = diff.x * diff.x + diff.y * diff.y;
	const float epsilon = 1.0f;

	// 방향 상태 업데이트
	if (std::abs(diff.x) > std::abs(diff.y))
		unit->dirState = (diff.x >= 0) ? APlayerCharacter::DirState::Right : APlayerCharacter::DirState::Left;
	else
		unit->dirState = (diff.y >= 0) ? APlayerCharacter::DirState::Bottom : APlayerCharacter::DirState::Top;

	// 속도
	const float speed = unit->MoveSpeed; // px/sec
	const float step = speed * deltaSeconds;
	if (len2 <= step * step || len2 <= epsilon * epsilon)
	{
		// 도착
		unit->SetActorLocation(target);
		m_movePathIndex++;
		if (m_movePathIndex >= (int)m_movePathWorld.size())
		{
			bIsUnitMoving = false;
			m_movePathWorld.clear();
			unit->animstate = APlayerCharacter::AnimationState::Idle;
		}
	}
	else
	{
		// 정규화 후 이동
		const float len = std::sqrt(len2);
		FVector2 dir = FVector2(diff.x / len, diff.y / len);
		unit->SetActorLocation(current + dir * step);
	}
}

void APlayerController::Release()
{
	OwnerScene.reset(); 
	for (auto& m_tile : m_tiles)
	{
		for (auto& tile : m_tile)
		{
			tile.reset();
		}
	}
	SelectedUnit.reset();
	for (auto& Unit : Units)
	{
		Unit.reset();
	}
	for (auto& Citie : Cities)
	{
		Citie.reset();
	}
	std::vector<std::vector<std::weak_ptr<ATile>>>().swap(m_tiles);
	std::vector<std::weak_ptr<APlayerCharacter>>().swap(Units);
	std::vector<std::weak_ptr<ACity>>().swap(Cities);
}

void APlayerController::SpawnAtIndex(int row, int col, int type, std::wstring name)
{
	if (const auto tile = m_tiles[col][row].lock())
	{
		if (tile->unit.expired() == false)
		{
			return;
		}
	}
	if (row < 0 || col < 0) return;

	//int type = static_cast<int>(EUnitType::Settler);
	if (auto OwnerSceneRef = std::dynamic_pointer_cast<UScene>(OwnerScene.lock()))
	{
		auto Unit = OwnerSceneRef->NewObject<APlayerCharacter>(APlayerCharacter::GetUnitTypeString(type) + std::to_wstring(Time::GetElapsedTime()) + name, ESCENELAYER::CHARACTER);
		if (auto UnitRef = Unit.lock())
		{
			UnitRef->SetName(APlayerCharacter::GetUnitTypeString(type).c_str());
			UnitRef->SetUnitType(type);
			UnitRef->Initialize();
			FVector2 pos = ATile::GetTilePositionAtIndex(col, row);
			FVector2 size = UnitRef->GetActorSize();
			UnitRef->row = row;
			UnitRef->col = col;
			if (const auto tile = m_tiles[col][row].lock())
			{
				tile->unit = UnitRef;
			}
			UnitRef->SetActorLocation(pos - size / 2);
			Units.push_back(UnitRef);
		}
	}	
}

void APlayerController::SpawnAtPosition(FVector2 position, EUnitType unitType)
{
	if (position.x < 0 || position.y < 0) return;
	int type = static_cast<int>(unitType);
	FVector2 index = ATile::GetIndexAtPosition(position);
	FVector2 pos = ATile::GetTilePositionAtIndex((int)index.x, (int)index.y);
	if (pos.x == -1 || pos.y == -1) return;

	if (const auto tile = m_tiles[(int)index.x][(int)index.y].lock())
	{
		if (tile->unit.expired() == false)
		{
			return;
		}
	}

	if (auto OwnerSceneRef = std::dynamic_pointer_cast<UScene>(OwnerScene.lock()))
	{
		auto Unit = OwnerSceneRef->NewObject<APlayerCharacter>(APlayerCharacter::GetUnitTypeString(type) + std::to_wstring(Time::GetElapsedTime()), ESCENELAYER::CHARACTER);
		if (auto UnitRef = Unit.lock())
		{
			UnitRef->SetName(APlayerCharacter::GetUnitTypeString(type).c_str());
			UnitRef->SetUnitType(type);
			UnitRef->Initialize();
			FVector2 size = UnitRef->GetActorSize();
			UnitRef->row = (int)index.y;
			UnitRef->col = (int)index.x;
			if (const auto tile = m_tiles[UnitRef->col][UnitRef->row].lock())
			{
				tile->unit = UnitRef;
			}
			UnitRef->SetActorLocation(pos - size / 2);
			Units.push_back(UnitRef);
		}
	}
}

std::weak_ptr<APlayerCharacter> APlayerController::GetUnitRefAtPosition(FVector2 position)
{
	FVector2 index = ATile::GetIndexAtPosition(position);

	int x = (int)index.x;
	int y = (int)index.y;

	if (x == -1 || y == -1) return std::weak_ptr<APlayerCharacter>();

	if (m_tiles[x][y].expired() == false)
		return m_tiles[x][y].lock()->unit;
	else
		return std::weak_ptr<APlayerCharacter>();
}

std::weak_ptr<ATile> APlayerController::GetTileRefAtPosition(FVector2 position)
{
	FVector2 index = ATile::GetIndexAtPosition(position);

	if (index.x == -1 || index.y == -1) return std::weak_ptr<ATile>();

	int x = (int)index.x;
	int y = (int)index.y;
	return m_tiles[x][y];
}

void APlayerController::HandleInput()
{
	// 이동 중에는 모든 입력 무시
	if (bIsUnitMoving) return;

	if (Input::IsKeyPressed(VK_K))
	{
		for (const auto& Unit : Units)
		{
			if (auto OwnerSceneRef = std::dynamic_pointer_cast<UScene>(OwnerScene.lock()))
			{
				OwnerSceneRef->Destroy(Unit);
			}
		}
	}

	if (Input::IsKeyPressed(VK_RBUTTON))
	{
		if (SelectedUnit.expired() == false)
		{
			MoveSelectedUnitTo(Input::GetMouseWorldPosition(Game::GetGameState()->GetMainCamera()));
		}
	}

	if (Input::IsKeyPressed(VK_LBUTTON))
	{
		SelectedUnit = GetUnitRefAtPosition(Input::GetMouseWorldPosition(Game::GetGameState()->GetMainCamera()));
		if (const auto SelectedUnitRef = SelectedUnit.lock())	// 선택한 블록이 캐릭터가 있다면
		{
			if (const auto OwnerSceneRef = OwnerScene.lock())
			{
				std::shared_ptr<UPlayScene> ps = std::dynamic_pointer_cast<UPlayScene>(OwnerSceneRef);

				if (auto widget = ps->m_PlayScene_Widget.lock())
				{
					if (auto text = widget->m_selectTileName.lock())
					{
                        text->SetContent(L"캐릭터 이름 : " + SelectedUnitRef->GetName());
					}
				}
				
				if (auto widget = ps->m_PlayScene_Widget.lock())
				{
					if (auto text = widget->m_selectTileActionCount.lock())
					{
                        text->SetContent(L"남은 행동 수 : " + std::to_wstring(SelectedUnitRef->ActionRemainCount));
					}
				}

				// 미리보기
				FVector2 unitIndex = ATile::GetIndexAtPosition(SelectedUnitRef->GetActorLocation());
				std::vector<std::pair<int, int>> reachable;
				ATile::GetReachableTiles(int(unitIndex.x), int(unitIndex.y), SelectedUnitRef->ActionRemainCount, m_tiles, reachable);
				if (reachable.empty())
				{
					for (auto& tiles : m_tiles)
					{
						for (auto& tile : tiles)
						{
							if (const auto tileRef = tile.lock())
							{
								tileRef->SetHighlight(false);
							}
							
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
							if (const auto tileRef = tile.lock())
							{
								tileRef->SetHighlight(false);
							}
						}
					}
                    for (auto& tile : reachable)
					{
						int r = tile.first;
						int c = tile.second;
                        // 현재 유닛이 서 있는 타일은 표시하지 않음
                        if (r == (int)unitIndex.x && c == (int)unitIndex.y) continue;
						if (const auto tileRef = m_tiles[r][c].lock())
						{
							tileRef->SetHighlight(true); // 미리보기 표시
						}
					}
				}
			}
		}
		else// 선택한 블록이 캐릭터가 없다면
		{
			for (auto& tiles : m_tiles)
			{
				for (auto& tile : tiles)
				{
					if (const auto tileRef = tile.lock())
					{
						tileRef->SetHighlight(false);
					}
				}
			}
			if (const auto OwnerSceneRef = OwnerScene.lock())
			{
				std::shared_ptr<UPlayScene> ps = std::dynamic_pointer_cast<UPlayScene>(OwnerSceneRef);

				std::weak_ptr<ATile> t = GetTileRefAtPosition(Input::GetMouseWorldPosition(Game::GetGameState()->GetMainCamera()));

				if (t.expired() == false)
				{

					if (auto widget = ps->m_PlayScene_Widget.lock())
					{
						if (auto text = widget->m_selectTileName.lock())
						{
                            text->SetContent(L"타일 이름 : " + t.lock()->GetTileName());
						}
					}
					if (auto widget = ps->m_PlayScene_Widget.lock())
					{
						if (auto text = widget->m_selectTileActionCount.lock())
						{
                            text->SetContent(L"");
						}
					}
				}
				else
				{
					if (auto widget = ps->m_PlayScene_Widget.lock())
					{
						if (auto text = widget->m_selectTileName.lock())
						{
                            text->SetContent(L"타일 이름 : ");
						}
					}
					if (auto widget = ps->m_PlayScene_Widget.lock())
					{
						if (auto text = widget->m_selectTileActionCount.lock())
						{
                            text->SetContent(L"");
						}
					}
				}
			}
		}
	}
}
