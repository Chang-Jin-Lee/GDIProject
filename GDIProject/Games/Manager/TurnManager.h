#pragma once
#include <memory>
#include <Classes/Object.h>
#include "../Core/GameConfig.h"
#include "../Core/GameCommand.h"
#include "../Core/MatchSettings.h"
#include "../Network/NetworkSession.h"
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
    void EndActiveTurn(bool sendNetworkCommand = true);
    void SkipSelectedUnit(bool sendNetworkCommand = true);
    void ApplyTimeoutPenalty(bool sendNetworkCommand = true);
    void ResolveAITurn();
    void PollNetwork();

    inline  ETurnState GetCurrentTurn() const { return CurrentTurn; }
    inline std::weak_ptr<APlayerController> GetPlayer() { return Player; }
    inline std::weak_ptr<APlayerController> GetPlayer2() { return Player2; }
    std::weak_ptr<APlayerController> GetActivePlayer();
    ETurnPhase GetCurrentPhase() const { return CurrentPhase; }
    int GetRemainingTurnSeconds() const { return RemainingTurnSeconds; }
    EPlayerSlot GetActiveSlot() const;

public:
    std::weak_ptr<UObject> OwnerScene;
    std::vector<std::vector<std::weak_ptr<ATile>>> m_tiles;

private:
    ETurnState CurrentTurn;
    ETurnPhase CurrentPhase = ETurnPhase::Player1Turn;
    GameConfig Config;
    double TurnTimerAccumulator = 0.0;
    int RemainingTurnSeconds = 30;
    bool bHandshakeSent = false;
    std::weak_ptr<APlayerController> Player;
    std::weak_ptr<APlayerController> Player2;
    std::unique_ptr<NetworkSession> Network;

    void SendNetworkCommand(GameCommand command);
    void ApplyRemoteCommand(const GameCommand& command);
};
