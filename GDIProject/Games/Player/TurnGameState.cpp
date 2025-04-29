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
    m_iTurnCount = 0;
}

void TurnGameState::PostInitialize()
{
    FFileHelper::LoadFileToArrayWithDelimeter<int>(L"Tile", L"Tiles.txt", L',', 100, &m_TileCloneInfoRowSize, &m_TileCloneInfoColsize, &m_TileCloneInfo);
    m_baseTileImage = new Gdiplus::Bitmap(L"../Resource/Tile/Tiles.png");
}

void TurnGameState::Release()
{

}

// static으로 나의 클래스를 부모에게 등록하기
bool TurnGameState::registered = []() {
GameStateBase::RegistGameState(L"AdGameState", []() -> GameStateBase* {
    return new TurnGameState();
    });
return true;
}();