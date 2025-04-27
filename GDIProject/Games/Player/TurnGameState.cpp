#include "TurnGameState.h"
#include <iostream>
#include <Runtime/Core/FIleHelper.h>

TurnGameState::TurnGameState()
{
    std::cout << "AdGameState created!\n";
}

void TurnGameState::Initialize() {
    std::cout << "AdGameState running!\n";
    m_gGameScore = 0;

    TurnMgr = std::make_shared<TurnManager>();
    TurnMgr->Initialize();
}

void TurnGameState::PostInitialize()
{
    FFileHelper::LoadFileToArrayWithDelimeter<int>(L"Tile", L"Tiles.txt", L',', 100, &m_TileCloneInfoRowSize, &m_TileCloneInfoColsize, &m_TileCloneInfo);
    m_baseTileImage = new Gdiplus::Bitmap(L"../Resource/Tile/Tiles.png");
}

void TurnGameState::Release()
{

}

void TurnGameState::CheckVictoryConditions()
{
    TurnCount++;

    // 1. 턴 수 제한
    if (TurnCount >= MaxTurn)
    {
        printf("턴 제한 도달! 승패 자동 판정!\n");
        bIsGameOver = true;
        // 점수 높은 쪽 승리 처리 가능
        return;
    }

    // 2. AI 모두 도시 없음 → 플레이어 승리
    bool bAllAIKilled = true;
    for (auto& ai : TurnMgr->GetAIPlayers())
    {
        if (!ai->Cities.empty())
        {
            bAllAIKilled = false;
            break;
        }
    }

    if (bAllAIKilled)
    {
        printf("🎉 플레이어 승리!\n");
        bIsGameOver = true;
        return;
    }

    // 3. 플레이어 도시 모두 파괴 → 플레이어 패배
    if (TurnMgr->GetPlayer()->Cities.empty())
    {
        printf("💀 플레이어 패배!\n");
        bIsGameOver = true;
        return;
    }
}

// static으로 나의 클래스를 부모에게 등록하기
bool TurnGameState::registered = []() {
GameStateBase::RegistGameState(L"AdGameState", []() -> GameStateBase* {
    return new TurnGameState();
    });
return true;
}();