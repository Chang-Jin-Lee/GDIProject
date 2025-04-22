#include "Pawn.h"

APawn::APawn()
{
	SetName((wchar_t*)L"APawn");
}

APawn::~APawn()
{

}

void APawn::Initialize()
{
	__super::Initialize();
}

void APawn::Update()
{
	__super::Update();
}

void APawn::Release()
{
	__super::Release();
}
