#include "dGameState.h"
#include <iostream>
#include <Runtime/Core/FIleHelper.h>

AdGameState::AdGameState()
{
    std::cout << "AdGameState created!\n";
}

void AdGameState::Initialize() {
    std::cout << "AdGameState running!\n";
    m_gGameScore = 0;
}

void AdGameState::PostInitialize()
{
    FFileHelper::LoadFileToArrayWithDelimeter<int>(L"Tile", L"Tiles.txt", L',', 100, &m_TileCloneInfoRowSize, &m_TileCloneInfoColsize, &m_TileCloneInfo);
    m_baseTileImage = new Gdiplus::Bitmap(L"../Resource/Tile/Tiles.bmp");
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