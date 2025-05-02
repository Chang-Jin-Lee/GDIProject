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
    void Release();

    inline  ETurnState GetCurrentTurn() const { return CurrentTurn; }
    inline std::shared_ptr<APlayerController> GetPlayer() { return Player; }

public:
    std::shared_ptr<UScene> OwnerScene;
    std::vector<std::vector<std::shared_ptr<ATile>>> m_tiles;
private:
    ETurnState CurrentTurn;
    std::shared_ptr<APlayerController> Player;
};
