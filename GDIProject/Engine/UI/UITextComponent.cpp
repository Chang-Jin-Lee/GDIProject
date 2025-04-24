#include "UITextComponent.h"
#include "../Classes/Camera/CameraActor.h"
#include "../Runtime/Renderer/Renderer.h"

void SUITextComponent::Initialize()
{
	__super::Initialize();
}

void SUITextComponent::Update()
{
	__super::Update();

	if (m_AttachedActor == nullptr) // 액터에 부착되어 있지 않을 때
	{
		Renderer::RenderTextUI(this, 0, 0);
	}
	else // 어떤 액터에 부착되어 있을 때
	{
		Renderer::RenderTextUI(this,m_AttachedActor->GetActorLocation().x,m_AttachedActor->GetActorLocation().y);
	}
}
void SUITextComponent::Release()
{
	__super::Release();
}

void SUITextComponent::AttachedUIToActor(AActor* actor)
{
	m_AttachedActor = actor; 
}
