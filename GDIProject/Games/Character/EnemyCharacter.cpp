#include "EnemyCharacter.h"
#include "../Games.h"
#include <Time/Time.h>
#include <Math/Math.h>

AEnemyCharacter::AEnemyCharacter()
{
	srand((unsigned int)Time::GetElapsedTime());
	m_randomWidth = 2.0f;
	m_randomHeight = 2.0f;

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
	float width = Renderer::GetResolution().x;
	float height = Renderer::GetResolution().y;
	SetActorSize(50, 50);
	SetActorLocation(FRandom::GetRandomInRange(width*0.2f, width), FRandom::GetRandomInRange(height*0.3f, height));
	SetActorScale(FRandom::GetRandomInRange(1.0f, m_randomWidth), FRandom::GetRandomInRange(1.0f, m_randomHeight));

	float uiwidth = 120;
	float uiheith = 20;
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
