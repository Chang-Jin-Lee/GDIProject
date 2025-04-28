#pragma once

#include <Classes/Scene/Scene.h>
#include <Classes/Components/StaticMeshComponent.h>
#include "../Image/BackGroundImage.h"
#include "../UI/MenuScene_StartGuide.h"

class UMenuScene : public UScene
{
public:
	UMenuScene();
	~UMenuScene();

	virtual void Initialize() override;
	virtual void Update() override;
	virtual void LoadData() override;
	virtual void Release() override;

	virtual void DeleteNullObjects() override;

	void UIInitialize();
	void UpdateInput();

	// delegate function
	void StartGame();
	void EndGame();

	std::shared_ptr<ABackGroundImage> m_image;
	std::shared_ptr<UMenuscene_StartGuide> m_MenuSceneWidget;
	//SUIButton* m_button; 버튼은 아직 실험단계
};