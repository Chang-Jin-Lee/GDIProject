#include "TurnManager.h"

TurnManager::TurnManager()
{
    CurrentTurn = ETurnState::PlayerTurn;
}

TurnManager::~TurnManager() 
{

}

void TurnManager::Initialize()
{
    Player = std::make_shared<APlayerController>();
    Player->Initialize();

    // AI 2명 추가
    for (int i = 0; i < 2; ++i)
    {
        auto ai = std::make_shared<AAIPlayer>();
        ai->Initialize();
        AIPlayers.push_back(ai);
    }
}

void TurnManager::Update()
{
    if (CurrentTurn == ETurnState::PlayerTurn)
    {
        Player->HandleInput();
    }
    // AI는 Update()는 굳이 매 프레임 하지 않음 (EndTurn()때 수행)
}

void TurnManager::EndTurn()
{
    if (CurrentTurn == ETurnState::PlayerTurn)
    {
        // 플레이어 턴 끝
        Player->UpdateTurn();

        // AI들 모두 행동
        for (auto& ai : AIPlayers)
        {
            ai->UpdateTurn();
        }

        CurrentTurn = ETurnState::PlayerTurn; // 플레이어 턴으로 다시 넘어옴
    }
}
