#include "CharacterNameWidget.h"
#include <UI/UITextComponent.h>

UCharacterNameWidget::UCharacterNameWidget()
{
	bAttached = true;
	m_nameUI = CreateDefaultSubobject<SUITextComponent>(TEXT("NameUI"));
    if (auto sp = m_nameUI.lock()) WidgetComponents.push_back(std::static_pointer_cast<UWidgetComponent>(sp));
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
