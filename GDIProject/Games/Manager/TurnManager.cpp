#include "TurnManager.h"

TurnManager::TurnManager()
{
    bStatic = true;
    CurrentTurn = ETurnState::PlayerTurn;
    Player = CreateDefaultSubobject<APlayerController>(TEXT("Player"));
    for (int i = 0; i < 2; ++i)
    {
        auto ai = CreateDefaultSubobject<AAIPlayer>(TEXT("AIPlayer_") + std::to_wstring(i));
        AIPlayers.push_back(ai);
    }
}

TurnManager::~TurnManager() 
{

}

void TurnManager::Initialize()
{
    if (Player)
    {
        Player->OwnerScene = OwnerScene;
        Player->Initialize();
    }

    // AI 2명 추가
    for (int i = 0; i < 2; ++i)
    {
        if (AIPlayers[i])
        {
            AIPlayers[i]->OwnerScene = OwnerScene;
            AIPlayers[i]->Initialize();
        }
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
