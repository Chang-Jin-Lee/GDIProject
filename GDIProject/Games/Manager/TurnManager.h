#pragma once
#include <memory>
#include <Classes/Object.h>
#include "../Player/PlayerController.h"
#include "../Player/AIPlayer.h"

enum class ETurnState
{
    PlayerTurn,
    AITurn,
    MAX
};

class UScene;

class TurnManager : public AActor
{
public:
    TurnManager();
    ~TurnManager();

    void Initialize();
    void Update();
    void EndTurn(); // "턴 끝내기" 버튼에서 호출

    inline  ETurnState GetCurrentTurn() const { return CurrentTurn; }
    inline std::shared_ptr<APlayerController> GetPlayer() { return Player; }
    inline std::vector<std::shared_ptr<AAIPlayer>>& GetAIPlayers() { return AIPlayers; }

public:
    std::shared_ptr<UScene> OwnerScene;
    std::vector<std::vector<std::shared_ptr<ATile>>> m_tiles;
private:
    ETurnState CurrentTurn;
    std::shared_ptr<APlayerController> Player;
    std::vector<std::shared_ptr<AAIPlayer>> AIPlayers;
};
