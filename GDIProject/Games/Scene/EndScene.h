#pragma once

#include <Classes/Scene/Scene.h>
#include "../Image/BackGroundImage.h"

class SUITextComponent;

class UEndScene : public UScene
{
public:
	UEndScene();
	~UEndScene();

	virtual void Initialize() override;
	virtual void Update() override;
	virtual void LoadData() override;
	virtual void Release() override;

	void UIInitialize();
	void UpdateInput();

	std::shared_ptr<SUITextComponent> m_scoreui;
	std::shared_ptr<SUITextComponent> m_scoreGuideui;

	std::shared_ptr<ABackGroundImage> m_image;
};