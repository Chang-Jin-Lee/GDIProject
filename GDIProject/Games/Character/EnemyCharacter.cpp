#include "EnemyCharacter.h"
#include "../Games.h"
#include <Time/Time.h>
#include <Math/Math.h>

AEnemyCharacter::AEnemyCharacter()
{
	srand(Time::GetElapsedTime());
	m_randomWidth = 2;
	m_randomHeight = 2;
}


AEnemyCharacter::~AEnemyCharacter()
{

}

void AEnemyCharacter::Initialize()
{
	__super::Initialize();
	Game::GetCurrentScene();
	LoadData(L"test", L"Rectangle.png");
	int width = Renderer::GetWidth();
	int height = Renderer::GetHeight();
	SetActorSize(50, 50);
	SetActorLocation(FRandom::GetRandomInRange(width/10, width), FRandom::GetRandomInRange(0, height));
	SetActorScale(FRandom::GetRandomInRange(1.0, m_randomWidth), FRandom::GetRandomInRange(1.0, m_randomHeight));
}

void AEnemyCharacter::Update()
{
	__super::Update();
	Renderer::RenderActor(this);
}

void AEnemyCharacter::Release()
{
	__super::Release();
}

void AEnemyCharacter::LoadData(std::wstring baseDir, std::wstring fileName)
{
	LoadStaticMeshData(baseDir, fileName);
}
