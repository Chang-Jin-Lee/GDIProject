#pragma once

#include <Classes/Scene/Scene.h>
#include <string>
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
	void ConnectGame();
	void HostGame();
	void EndGame();
	void UpdateIpInput();

	std::weak_ptr<ABackGroundImage> m_image;
	std::weak_ptr<UMenuscene_StartGuide> m_MenuSceneWidget;
	std::string InputIp = "127.0.0.1";
	bool bConnectInputMode = false;
};