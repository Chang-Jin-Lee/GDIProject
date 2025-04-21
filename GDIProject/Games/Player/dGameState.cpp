#include "dGameState.h"
#include <iostream>

AdGameState::AdGameState()
{
    std::cout << "AdGameState created!\n";
}

void AdGameState::Initialize() {
    std::cout << "AdGameState running!\n";
    GameScore = 0;
}

void AdGameState::Release()
{

}

bool AdGameState::registered = []() {
AGameStateBase::RegistGameState(L"AdGameState", []() -> AGameStateBase* {
    return new AdGameState();
    });
return true;
}();