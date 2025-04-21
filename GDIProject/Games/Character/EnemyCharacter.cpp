#include "EnemyCharacter.h"
#include "../Games.h"
#include <Time/Time.h>
#include <Math/Math.h>

AEnemyCharacter::AEnemyCharacter()
{
	srand(Time::GetElapsedTime());
	m_randomWidth = 2;
	m_randomHeight = 2;

	m_textui = new SUIText();
}


AEnemyCharacter::~AEnemyCharacter()
{
	delete m_textui;
}

void AEnemyCharacter::Initialize()
{
	__super::Initialize();
	Game::GetCurrentScene();
	LoadData(L"test", L"Rectangle.png");
	int width = Renderer::GetResolution().x;
	int height = Renderer::GetResolution().y;
	SetActorSize(50, 50);
	SetActorLocation(FRandom::GetRandomInRange(width*0.2, width), FRandom::GetRandomInRange(height*0.3, height));
	SetActorScale(FRandom::GetRandomInRange(1.0, m_randomWidth), FRandom::GetRandomInRange(1.0, m_randomHeight));

	int uiwidth = 120;
	int uiheith = 20;
	m_textui->Initialize
	(
		GetName(),
		10,
		(wchar_t*)L"Verdana",
		Gdiplus::Color(255, 255, 255),
		FVector2(-50, -20),
		FVector2(uiwidth, uiheith),
		Gdiplus::FontStyleBold,
		Gdiplus::UnitPoint,
		Gdiplus::StringAlignmentNear,
		Gdiplus::StringAlignmentNear,
		Gdiplus::StringTrimmingNone
	);
}

void AEnemyCharacter::Update()
{
	__super::Update();
	Renderer::RenderActorWithUI(this, m_textui);
}

void AEnemyCharacter::Release()
{
	__super::Release();
}

void AEnemyCharacter::LoadData(std::wstring baseDir, std::wstring fileName)
{
	LoadStaticMeshData(baseDir, fileName);
}
