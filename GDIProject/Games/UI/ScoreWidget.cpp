#include "ScoreWidget.h"
#include "../Games.h"

UScoreWidget::UScoreWidget()
{
	m_scoreui = CreateDefaultSubobject<SUITextComponent>(TEXT("scoreui"));
	WidgetComponents.push_back(m_scoreui);
}

UScoreWidget::~UScoreWidget()
{
}

void UScoreWidget::Initialize()
{
	__super::Initialize();
	

	m_scoreui->Initialize
	(
		L"",
		10,
		(wchar_t*)L"Verdana",
		Gdiplus::Color(255, 255, 255),
		FVector2(int(Renderer::GetResolution().x * 0.5), int(Renderer::GetResolution().y * 0.1)),
		FVector2(250, 30)
	);
	m_scoreui->AttachedUIToActor(Game::GetGameState()->GetMainCamera().get());
	m_scoreui->m_content = _wcsdup(std::to_wstring(10.0f).c_str());
}

void UScoreWidget::Update()
{
	__super::Update();

}

void UScoreWidget::Release()
{
	__super::Release();
}
