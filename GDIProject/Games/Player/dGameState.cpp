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

// static으로 나의 클래스를 부모에게 등록하기
bool AdGameState::registered = []() {
AGameStateBase::RegistGameState(L"AdGameState", []() -> AGameStateBase* {
    return new AdGameState();
    });
return true;
}();