#include "CharacterNameWidget.h"

UCharacterNameWidget::UCharacterNameWidget()
{
	bAttached = true;
	m_nameUI = CreateDefaultSubobject<SUITextComponent>(TEXT("NameUI"));
	WidgetComponents.push_back(m_nameUI);
}

UCharacterNameWidget::~UCharacterNameWidget()
{

}

void UCharacterNameWidget::Initialize()
{

}

void UCharacterNameWidget::Update()
{

}

void UCharacterNameWidget::Release()
{

}
