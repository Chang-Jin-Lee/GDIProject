#include "PlayerController.h"
#include <Time/Time.h>
#include <Classes/Scene/Scene.h>
#include <Runtime/Renderer/Renderer.h>
#include "TurnGameState.h"
#include "../Games.h"
#include "../Core/GameConfig.h"
#include <Input/Input.h>
#include <iostream>
#include <utility>
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
	InitializeForSlot(EPlayerSlot::Player1);
}

void APlayerController::InitializeForSlot(EPlayerSlot slot)
{
	Slot = slot;
	SpawnStartingUnits();
}

void APlayerController::SetCommandSink(std::function<void(GameCommand)> sink)
{
	CommandSink = std::move(sink);
}

void APlayerController::SpawnStartingUnits()
{
	const GameConfig config = GameConfig::LoadFromResource();
	std::vector<std::pair<int, int>> occupied;
	const int minCol = (Slot == EPlayerSlot::Player2) ? TILE_COL_SIZE - 3 : 0;
	const int maxCol = (Slot == EPlayerSlot::Player2) ? TILE_COL_SIZE : 3;

	for (EUnitType unitType : config.GameRules.StartingUnits)
	{
		int row = static_cast<int>(FRandom::GetRandomInRange(0, TILE_ROW_SIZE));
		int col = static_cast<int>(FRandom::GetRandomInRange(minCol, maxCol));
		int guard = TILE_ROW_SIZE * TILE_COL_SIZE;
		while (guard-- > 0)
		{
			const bool alreadyPicked = std::find(occupied.begin(), occupied.end(), std::make_pair(row, col)) != occupied.end();
			const bool occupiedByUnit = ATile::IsValidIndex(row, col) && m_tiles[row][col].expired() == false && m_tiles[row][col].lock()->unit.expired() == false;
			if (!alreadyPicked && !occupiedByUnit)
			{
				break;
			}
			row = static_cast<int>(FRandom::GetRandomInRange(0, TILE_ROW_SIZE));
			col = static_cast<int>(FRandom::GetRandomInRange(minCol, maxCol));
		}

		occupied.emplace_back(row, col);
		SpawnAtIndex(row, col, static_cast<int>(unitType), std::to_wstring(row) + L"_" + std::to_wstring(col));
	}
}
void APlayerController::MoveSelectedUnitTo(const FVector2& pos)
{
	if (const auto& SelectedUnitRef = SelectedUnit.lock())
	{
		if (bIsUnitMoving) return; // �̵� �߿��� ���ο� ���� ����
		// ���� ������ Ÿ�� �ε����� ���� �߽� �������� ����Ͽ� ���� ����
		FVector2 unitCenter = SelectedUnitRef->GetActorLocation() + SelectedUnitRef->GetActorSize() / 2;
		FVector2 unitIndex = ATile::GetIndexAtPosition(unitCenter);
		if (unitIndex.x == -1 || unitIndex.y == -1) return;
		FVector2 index = ATile::GetIndexAtPosition(pos);
		FVector2 pos = ATile::GetTilePositionAtIndex((int)index.x, (int)index.y);
		if (pos.x == -1 || pos.y == -1) return;
		// ���� Ÿ���̸� �̵� ����
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
            // ���� Ÿ���̸� �̵� ����
            if (distance == 0) return;
            if (distance <= SelectedUnitRef->ActionRemainCount)
			{
				// ��� ����: ���� ��ġ���� ���������� ����� �����ϴ� �̿��� ���� ����
				m_movePathWorld.clear();
				m_movePathIndex = 0;
				int curRow = (int)unitIndex.x;
				int curCol = (int)unitIndex.y;
				const int targetRow = x;
				const int targetCol = y;
				int guard = TILE_ROW_SIZE * TILE_COL_SIZE; // ���� ����
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
							// ���� ��� ��� �ܰ迡���� Ÿ�ٸ� ���������� ���
							if (t->unit.expired() == false && !(nr == targetRow && nc == targetCol)) continue;
						}
						const int cost = ATile::SearchCost(nr, nc, targetRow, targetCol, m_tiles);
						if (cost != -1 && cost < bestCost)
						{
							bestCost = cost;
							bestRow = nr; bestCol = nc;
						}
					}
					// �� �̻� ���� �Ұ�
					if (bestRow == curRow && bestCol == curCol) break;
					curRow = bestRow; curCol = bestCol;
					FVector2 wpCenter = ATile::GetTilePositionAtIndex(curRow, curCol);
					FVector2 halfSize = SelectedUnitRef->GetActorSize() / 2;
					m_movePathWorld.push_back(wpCenter - halfSize);
				}

				// ��ΰ� ��ȿ�� ���� Ÿ�� ����/���� �� �̵� ����
				if (!m_movePathWorld.empty())
				{
					// ������ Ÿ�Ͽ� ���� ���� ����
					if (const auto tile = m_tiles[x][y].lock()) { tile->unit = SelectedUnit; }
					// ��� Ÿ�� ����
					if (const auto tile = m_tiles[(int)unitIndex.x][(int)unitIndex.y].lock()) { tile->unit.reset(); }
				}

				// �ൿ�� ���� �� UI ������Ʈ
				SelectedUnitRef->ActionRemainCount -= distance;
				SelectedUnitRef->row = x;
				SelectedUnitRef->col = y;
				if (!bSuppressCommandSink && CommandSink)
				{
					CommandSink(GameCommand::Move(0, Slot, SelectedUnitRef->StableId, x, y));
				}
				if (const auto OwnerSceneRef = OwnerScene.lock())
				{
					std::shared_ptr<UPlayScene> ps = std::dynamic_pointer_cast<UPlayScene>(OwnerSceneRef);
					if (auto widget = ps->m_PlayScene_Widget.lock())
					{
						if (auto text = widget->m_selectTileName.lock())
						{
							text->SetContent(L"\uCE90\uB9AD\uD130 \uC774\uB984 : " + SelectedUnitRef->GetName());
						}
						if (auto text2 = widget->m_selectTileActionCount.lock())
						{
							text2->SetContent(L"\uB0A8\uC740 \uD589\uB3D9 \uC218 : " + std::to_wstring(SelectedUnitRef->ActionRemainCount));
						}
					}
				}

				// ���̶���Ʈ ����
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

				// �̵� ����
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

	// ���� ���� ������Ʈ
	if (std::abs(diff.x) > std::abs(diff.y))
		unit->dirState = (diff.x >= 0) ? APlayerCharacter::DirState::Right : APlayerCharacter::DirState::Left;
	else
		unit->dirState = (diff.y >= 0) ? APlayerCharacter::DirState::Bottom : APlayerCharacter::DirState::Top;

	// �ӵ�
	const float speed = unit->MoveSpeed; // px/sec
	const float step = speed * deltaSeconds;
	if (len2 <= step * step || len2 <= epsilon * epsilon)
	{
		// ����
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
		// ����ȭ �� �̵�
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
	if (!ATile::IsValidIndex(row, col)) return;
	if (const auto tile = m_tiles[row][col].lock())
	{
		if (tile->unit.expired() == false)
		{
			return;
		}
	}

	const EUnitType unitType = static_cast<EUnitType>(type);
	if (auto OwnerSceneRef = std::dynamic_pointer_cast<UScene>(OwnerScene.lock()))
	{
		auto Unit = OwnerSceneRef->NewObject<APlayerCharacter>(APlayerCharacter::GetUnitTypeString(type) + std::to_wstring(Time::GetElapsedTime()) + name, ESCENELAYER::CHARACTER);
		if (auto UnitRef = Unit.lock())
		{
			int sameTypeIndex = 0;
			for (const auto& existing : Units)
			{
				if (auto existingRef = existing.lock())
				{
					if (existingRef->OwnerSlot == Slot && existingRef->UnitType == unitType)
					{
						++sameTypeIndex;
					}
				}
			}

			UnitRef->SetName(APlayerCharacter::GetUnitTypeString(type).c_str());
			UnitRef->SetUnitType(type);
			UnitRef->OwnerSlot = Slot;
			UnitRef->StableId = std::string(Slot == EPlayerSlot::Player2 ? "P2_" : "P1_") + ToString(unitType) + "_" + std::to_string(sameTypeIndex);
			UnitRef->Initialize();
			FVector2 pos = ATile::GetTilePositionAtIndex(row, col);
			FVector2 size = UnitRef->GetActorSize();
			UnitRef->row = row;
			UnitRef->col = col;
			if (const auto tile = m_tiles[row][col].lock())
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
			UnitRef->OwnerSlot = Slot;
			UnitRef->StableId = std::string(Slot == EPlayerSlot::Player2 ? "P2_" : "P1_") + ToString(unitType) + "_" + std::to_string(Units.size());
			UnitRef->Initialize();
			FVector2 size = UnitRef->GetActorSize();
			UnitRef->row = (int)index.x;
			UnitRef->col = (int)index.y;
			if (const auto tile = m_tiles[UnitRef->row][UnitRef->col].lock())
			{
				tile->unit = UnitRef;
			}
			UnitRef->SetActorLocation(pos - size / 2);
			Units.push_back(UnitRef);
		}
	}
}

std::shared_ptr<APlayerCharacter> APlayerController::FindUnitByStableId(const std::string& stableId) const
{
	for (const auto& unit : Units)
	{
		if (auto unitRef = unit.lock())
		{
			if (unitRef->StableId == stableId && !unitRef->bIsDead)
			{
				return unitRef;
			}
		}
	}
	return nullptr;
}

std::shared_ptr<APlayerCharacter> APlayerController::FindUnitAtIndex(int row, int col) const
{
	if (!ATile::IsValidIndex(row, col)) return nullptr;
	if (auto tile = m_tiles[row][col].lock())
	{
		return tile->unit.lock();
	}
	return nullptr;
}

bool APlayerController::TryMoveUnitToIndex(std::shared_ptr<APlayerCharacter> unit, int row, int col, bool animate, bool emitCommand)
{
	if (!unit || unit->bIsDead || !ATile::IsValidIndex(row, col)) return false;
	if (auto targetTile = m_tiles[row][col].lock())
	{
		if (targetTile->unit.expired() == false) return false;
		if (!targetTile->bVisible) return false;
	}

	FVector2 unitCenter = unit->GetActorLocation() + unit->GetActorSize() / 2;
	FVector2 currentIndex = ATile::GetIndexAtPosition(unitCenter);
	const int currentRow = static_cast<int>(currentIndex.x);
	const int currentCol = static_cast<int>(currentIndex.y);
	if (!ATile::IsValidIndex(currentRow, currentCol)) return false;
	if (currentRow == row && currentCol == col) return false;

	const int distance = ATile::SearchCost(currentRow, currentCol, row, col, m_tiles);
	if (distance < 0 || distance > unit->ActionRemainCount) return false;

	if (animate)
	{
		const bool oldSuppress = bSuppressCommandSink;
		bSuppressCommandSink = !emitCommand;
		SelectedUnit = unit;
		MoveSelectedUnitTo(ATile::GetTilePositionAtIndex(row, col));
		bSuppressCommandSink = oldSuppress;
		return true;
	}

	if (auto oldTile = m_tiles[currentRow][currentCol].lock()) oldTile->unit.reset();
	if (auto newTile = m_tiles[row][col].lock()) newTile->unit = unit;
	unit->ActionRemainCount -= distance;
	unit->row = row;
	unit->col = col;
	unit->SetActorLocation(ATile::GetTilePositionAtIndex(row, col) - unit->GetActorSize() / 2);
	if (emitCommand && !bSuppressCommandSink && CommandSink)
	{
		CommandSink(GameCommand::Move(0, Slot, unit->StableId, row, col));
	}
	return true;
}

bool APlayerController::TryAttackUnit(std::shared_ptr<APlayerCharacter> attacker, std::shared_ptr<APlayerCharacter> target, bool emitCommand)
{
	if (!attacker || !target || attacker->bIsDead || target->bIsDead) return false;
	if (attacker->OwnerSlot == target->OwnerSlot) return false;
	if (attacker->ActionRemainCount <= 0) return false;

	const int distance = std::abs(attacker->row - target->row) + std::abs(attacker->col - target->col);
	if (distance > attacker->AttackRange) return false;
	if (distance <= 1 && !attacker->CanMelee && !attacker->CanRanged) return false;
	if (distance > 1 && !attacker->CanRanged) return false;

	attacker->Attack(target.get());
	attacker->ActionRemainCount = 0;
	if (emitCommand && !bSuppressCommandSink && CommandSink)
	{
		CommandSink(GameCommand::Attack(0, Slot, attacker->StableId, target->StableId));
	}
	if (target->bIsDead && ATile::IsValidIndex(target->row, target->col))
	{
		if (auto tile = m_tiles[target->row][target->col].lock()) tile->unit.reset();
	}
	return true;
}

bool APlayerController::HasLivingUnits() const
{
	for (const auto& unit : Units)
	{
		if (auto unitRef = unit.lock())
		{
			if (!unitRef->bIsDead) return true;
		}
	}
	return false;
}

std::vector<std::shared_ptr<APlayerCharacter>> APlayerController::GetLivingUnits() const
{
	std::vector<std::shared_ptr<APlayerCharacter>> living;
	for (const auto& unit : Units)
	{
		if (auto unitRef = unit.lock())
		{
			if (!unitRef->bIsDead) living.push_back(unitRef);
		}
	}
	return living;
}

void APlayerController::ReadyUnitsForNextTurn()
{
	for (const auto& unit : Units)
	{
		if (auto unitRef = unit.lock())
		{
			if (!unitRef->bIsDead) unitRef->ReadyForNextTurn();
		}
	}
}

void APlayerController::SkipRemainingUnits()
{
	for (const auto& unit : Units)
	{
		if (auto unitRef = unit.lock())
		{
			unitRef->ActionRemainCount = 0;
			unitRef->bSkipTurn = true;
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
	// �̵� �߿��� ��� �Է� ����
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
			const FVector2 mouseWorld = Input::GetMouseWorldPosition(Game::GetGameState()->GetMainCamera());
			if (auto selected = SelectedUnit.lock())
			{
				if (auto tile = GetTileRefAtPosition(mouseWorld).lock())
				{
					if (auto target = tile->unit.lock())
					{
						if (target->OwnerSlot != Slot && TryAttackUnit(selected, target, true))
						{
							return;
						}
					}
				}
			}
			MoveSelectedUnitTo(mouseWorld);
		}
	}

	if (Input::IsKeyPressed(VK_LBUTTON))
	{
		SelectedUnit = GetUnitRefAtPosition(Input::GetMouseWorldPosition(Game::GetGameState()->GetMainCamera()));
		if (const auto SelectedUnitRef = SelectedUnit.lock())	// ������ ������ ĳ���Ͱ� �ִٸ�
		{
			if (const auto OwnerSceneRef = OwnerScene.lock())
			{
				std::shared_ptr<UPlayScene> ps = std::dynamic_pointer_cast<UPlayScene>(OwnerSceneRef);

				if (auto widget = ps->m_PlayScene_Widget.lock())
				{
					if (auto text = widget->m_selectTileName.lock())
					{
                        text->SetContent(L"\uCE90\uB9AD\uD130 \uC774\uB984 : " + SelectedUnitRef->GetName());
					}
				}
				
				if (auto widget = ps->m_PlayScene_Widget.lock())
				{
					if (auto text = widget->m_selectTileActionCount.lock())
					{
                        text->SetContent(L"\uB0A8\uC740 \uD589\uB3D9 \uC218 : " + std::to_wstring(SelectedUnitRef->ActionRemainCount));
					}
				}

				// �̸�����
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
                        // ���� ������ �� �ִ� Ÿ���� ǥ������ ����
                        if (r == (int)unitIndex.x && c == (int)unitIndex.y) continue;
						if (const auto tileRef = m_tiles[r][c].lock())
						{
							tileRef->SetHighlight(true); // �̸����� ǥ��
						}
					}
				}
			}
		}
		else// ������ ������ ĳ���Ͱ� ���ٸ�
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
                            text->SetContent(L"\uD0C0\uC77C \uC774\uB984 : " + t.lock()->GetTileName());
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
                            text->SetContent(L"\uD0C0\uC77C \uC774\uB984 : ");
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
