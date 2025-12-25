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
    inline std::weak_ptr<APlayerController> GetPlayer() { return Player; }

public:
    std::weak_ptr<UObject> OwnerScene;
    std::vector<std::vector<std::weak_ptr<ATile>>> m_tiles;

private:
    ETurnState CurrentTurn;
    std::weak_ptr<APlayerController> Player;
};
