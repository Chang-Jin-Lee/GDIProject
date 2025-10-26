#include "EnemyCharacter.h"
#include "../Games.h"
#include <Time/Time.h>
#include <Math/Math.h>
#include <UI/UITextComponent.h>
#include <gdiplus.h>

AEnemyCharacter::AEnemyCharacter()
{
	bStatic = false;
	srand((unsigned int)Time::GetElapsedTime());
	m_randomWidth = 2.0f;
	m_randomHeight = 2.0f;
	m_textui = CreateDefaultSubobject<SUITextComponent>(TEXT("m_textui"));
}


AEnemyCharacter::~AEnemyCharacter()
{
	m_textui.reset();
}

void AEnemyCharacter::Initialize()
{
	__super::Initialize();
	LoadData(L"test", L"Rectangle.png");
	SetActorLocation(FRandom::GetRandomInRange(Renderer::GetResolution().x * 0.2f, Renderer::GetResolution().y), FRandom::GetRandomInRange(Renderer::GetResolution().y * 0.3f, Renderer::GetResolution().y));
	if (auto text = Cast<SUITextComponent>(m_textui))
	{
		text->Initialize(GetName(), 10, (wchar_t*)L"Verdana", Gdiplus::Color(255, 255, 255), FVector2(-60 + GetActorSize().x * GetActorScale().x / 2, -20), FVector2(120.0f, 20.0f));
		text->AttachedUIToActor(weak_from_this());
	}	
}

void AEnemyCharacter::Update()
{
	__super::Update();
}

void AEnemyCharacter::Release()
{
	__super::Release();
}

void AEnemyCharacter::LoadData(std::wstring baseDir, std::wstring fileName)
{
	LoadStaticMeshData(baseDir, fileName);
}
