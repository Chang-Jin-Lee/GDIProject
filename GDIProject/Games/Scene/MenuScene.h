#pragma once

#include <Classes/Scene/Scene.h>
#include <Classes/Components/StaticMeshComponent.h>
#include "../Image/BackGroundImage.h"
#include <UI/UIButton.h>
#include <UI/UIText.h>

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

	ABackGroundImage* m_image;
	SUIText* m_startGuideui;

	//SUIButton* m_button; 버튼은 아직 실험단계
};