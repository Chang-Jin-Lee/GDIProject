#include "AIPlayer.h"

AAIPlayer::AAIPlayer() {}

AAIPlayer::~AAIPlayer() {}

void AAIPlayer::Initialize()
{
    // 초기 개척자 한 명 소환
    //auto Settler = std::make_shared<AUnit>(EUnitType::Settler, this);
    auto Settler = std::make_shared<AUnit>(EUnitType::Settler);
    Settler->SetActorLocation(FVector2(5, 5)); // 시작 위치
    Units.push_back(Settler);
}

void AAIPlayer::UpdateTurn()
{
    UpdateUnits();
    UpdateCities();
}

void AAIPlayer::UpdateUnits()
{
    for (auto& unit : Units)
    {
        if (unit->bIsDead)
            continue;

        if (unit->UnitType == EUnitType::Settler)
        {
            HandleSettler(unit);
        }
        else
        {
            HandleCombatUnit(unit);
        }
    }

    // 죽은 유닛 제거
    Units.erase(
        std::remove_if(Units.begin(), Units.end(), [](auto& u) { return u->bIsDead; }),
        Units.end()
    );
}

void AAIPlayer::UpdateCities()
{
    for (auto& city : Cities)
    {
        city->ProductionPoints--;

        if (city->ProductionPoints <= 0)
        {
            // 간단한 로직: 전사 생산
            auto unit = std::make_shared<AUnit>(EUnitType::Warrior);
            unit->SetActorLocation(city->GetActorLocation());
            Units.push_back(unit);

            city->ProductionPoints = 3; // 다시 생산 대기
        }
    }
}

void AAIPlayer::HandleSettler(std::shared_ptr<AUnit> Settler)
{
    //auto tile = GetTileAtPosition(Settler->GetActorLocation());
    //
    //if (!tile || tile->HasCity)
    //{
    //    FVector2 bestTile = FindNearbyGoodTile(Settler->GetLocation());
    //    Settler->MoveTo(bestTile);
    //    return;
    //}
    //
    //if (tile->m_etileType == ETileType::Grassland || tile->m_etileType == ETileType::Plain)
    //{
    //    // 도시 건설
    //    auto city = std::make_shared<ACity>();
    //    city->SetLocation(Settler->GetLocation());
    //    Cities.push_back(city);
    //
    //    tile->HasCity = true;
    //    Settler->bIsDead = true;
    //}
    //else
    //{
    //    FVector2 bestTile = FindNearbyGoodTile(Settler->GetLocation());
    //    Settler->MoveTo(bestTile);
   // }
}

void AAIPlayer::HandleCombatUnit(std::shared_ptr<AUnit> CombatUnit)
{
    //auto enemy = FindNearestEnemy(CombatUnit->GetLocation());
    //if (!enemy)
    //    return;
    //
    //float distance = FVector2::Distance(CombatUnit->GetLocation(), enemy->GetLocation());
    //
    //if (distance <= 1.5f)
    //{
    //    // 인접 → 공격
    //    CombatUnit->Attack(enemy);
    //}
    //else
    //{
    //    // 아니면 이동
    //    CombatUnit->MoveTo(enemy->GetLocation());
    //}
}

FVector2 AAIPlayer::FindNearbyGoodTile(const FVector2& currentPos)
{
    const int SearchRange = 3;
    FVector2 bestPos = currentPos;
    //int bestScore = -1;
    //
    //for (int dx = -SearchRange; dx <= SearchRange; dx++)
    //{
    //    for (int dy = -SearchRange; dy <= SearchRange; dy++)
    //    {
    //        int nx = GetTileX(currentPos) + dx;
    //        int ny = GetTileY(currentPos) + dy;
    //
    //        if (!IsValidTile(nx, ny))
    //            continue;
    //
    //        auto tile = m_tiles[nx][ny];
    //        if (!tile || tile->HasCity)
    //            continue;
    //
    //        int score = EvaluateTile(tile);
    //        if (score > bestScore)
    //        {
    //            bestScore = score;
    //            bestPos = tile->GetLocation();
    //        }
    //    }
    //}

    return bestPos;
}

int AAIPlayer::EvaluateTile(std::shared_ptr<ATile> tile)
{
    if (tile->m_etileType == ETileType::Grassland) return 10;
    if (tile->m_etileType == ETileType::Plain) return 8;
    if (tile->m_etileType == ETileType::Hills) return 5;
    return 1;
}
