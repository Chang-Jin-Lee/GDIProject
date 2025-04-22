#pragma once

#include <Classes/Scene/Scene.h>
#include <Classes/Components/StaticMeshComponent.h>
#include "../Image/BackGroundImage.h"

class SUITextComponent;

class UMenuScene : public UScene
{
public:
	UMenuScene();
	~UMenuScene();

	virtual void Initialize() override;
	virtual void Update() override;
	virtual void LoadData() override;
	virtual void Release() override;

	void UIInitialize();
	void UpdateInput();

	std::shared_ptr<ABackGroundImage> m_image;
	std::shared_ptr<SUITextComponent> m_startGuideui;
	//SUIButton* m_button; 버튼은 아직 실험단계
};