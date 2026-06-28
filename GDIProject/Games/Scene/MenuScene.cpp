#include "MenuScene.h"
#include <Runtime/Renderer/Renderer.h>
#include <Input/Input.h>
#include "../Games.h"
#include "../Core/GameConfig.h"
#include "../Core/MatchSettings.h"
#include <Classes/Scene/Scene.h>
#include "PlayScene.h"
#include <Experiment/SmartCast.h>
#include <Runtime/Core/FIleHelper.h>
#include <UI/UIButtonComponent.h>
#include <UI/UITextComponent.h>

UMenuScene::UMenuScene()
{
	m_image = NewObject<ABackGroundImage>(TEXT("m_image"));
	m_MenuSceneWidget = CreateWidget<UMenuscene_StartGuide>(TEXT("m_MenuSceneWidget"), EUILAYER::HUD);

	if (const auto widgetRef = Cast<UMenuscene_StartGuide>(m_MenuSceneWidget))
	{
		if (auto btn = Cast<SUIButtonComponent>(widgetRef->m_startGameButton))
		{
			btn->SetVoidDelegate([this]() { StartGame(); });
		}
		if (auto btn = Cast<SUIButtonComponent>(widgetRef->m_connectGameButton))
		{
			btn->SetVoidDelegate([this]() { ConnectGame(); });
		}
		if (auto btn = Cast<SUIButtonComponent>(widgetRef->m_hostGameButton))
		{
			btn->SetVoidDelegate([this]() { HostGame(); });
		}
		if (auto btn = Cast<SUIButtonComponent>(widgetRef->m_endGameButton))
		{
			btn->SetVoidDelegate([this]() { EndGame(); });
		}
	}
}

UMenuScene::~UMenuScene()
{
	m_image.reset();
	m_MenuSceneWidget.reset();
}

void UMenuScene::Initialize()
{
	__super::Initialize();
	if (auto cameraRef = Cast<ACameraActor>(Game::GetGameState()->GetMainCamera()))
	{
		cameraRef->SetCameraLocation(FVector2(0, 0));
	}
	UIInitialize();
}

void UMenuScene::Update()
{
	__super::Update();
	UpdateInput();
}

void UMenuScene::LoadData()
{
	__super::LoadData();
	if (auto imageRef = Cast<ABackGroundImage>(m_image))
	{
		imageRef->LoadData(L"Image", L"TitleImage.png");
		imageRef->SetActorSize(Renderer::GetResolution().x, Renderer::GetResolution().y);
	}
}

void UMenuScene::Release()
{
	__super::Release();
	m_image.reset();
	m_MenuSceneWidget.reset();
}

void UMenuScene::DeleteNullObjects()
{
	__super::DeleteNullObjects();
}

void UMenuScene::UIInitialize()
{
	if (auto widgetRef = Cast<UMenuscene_StartGuide>(m_MenuSceneWidget))
	{
		widgetRef->Initialize();
	}
}

void UMenuScene::UpdateInput()
{
	UpdateIpInput();

	if (Input::IsKeyPressed(VK_C))
	{
		StartGame();
	}

	if (Input::IsKeyDown(VK_R))
	{
		if (auto imageRef = Cast<ABackGroundImage>(m_image))
		{
			const float& rotation = imageRef->GetActorRotation();
			imageRef->SetActorRotation(rotation + Time::GetElapsedTime() * 20);
		}
	}
}
void UMenuScene::StartGame()
{
	ResetMatchSettings();
	GetMutableMatchSettings().Mode = EMatchMode::SinglePlayer;
	UScene::ChangeScene<UPlayScene>(Game::GetNextSceneSharedPtr(), Game::GetNextSceneWeakPtr());
}

void UMenuScene::ConnectGame()
{
	bConnectInputMode = true;
	MatchSettings& settings = GetMutableMatchSettings();
	settings.Mode = EMatchMode::MultiplayerClient;
	settings.RemoteIp = InputIp.empty() ? "127.0.0.1" : InputIp;
	settings.Port = GameConfig::LoadFromResource().NetworkRules.Port;
	if (auto widgetRef = Cast<UMenuscene_StartGuide>(m_MenuSceneWidget))
	{
		if (auto status = Cast<SUITextComponent>(widgetRef->m_statusText)) status->SetContent(L"IP 입력 후 Enter로 접속");
	}
}

void UMenuScene::HostGame()
{
	MatchSettings& settings = GetMutableMatchSettings();
	settings.Mode = EMatchMode::MultiplayerHost;
	settings.Port = GameConfig::LoadFromResource().NetworkRules.Port;
	settings.HostWaiting = true;
	UScene::ChangeScene<UPlayScene>(Game::GetNextSceneSharedPtr(), Game::GetNextSceneWeakPtr());
}

void UMenuScene::UpdateIpInput()
{
	if (!bConnectInputMode)
	{
		return;
	}

	for (int key = VK_0; key <= VK_9; ++key)
	{
		if (Input::IsKeyPressed(key))
		{
			InputIp.push_back(static_cast<char>('0' + (key - VK_0)));
		}
	}
	if (Input::IsKeyPressed(0xBE) && InputIp.size() < 15)
	{
		InputIp.push_back('.');
	}
	if (Input::IsKeyPressed(VK_BACK) && !InputIp.empty())
	{
		InputIp.pop_back();
	}
	if (Input::IsKeyPressed(VK_RETURN))
	{
		MatchSettings& settings = GetMutableMatchSettings();
		settings.Mode = EMatchMode::MultiplayerClient;
		settings.RemoteIp = InputIp.empty() ? "127.0.0.1" : InputIp;
		settings.Port = GameConfig::LoadFromResource().NetworkRules.Port;
		UScene::ChangeScene<UPlayScene>(Game::GetNextSceneSharedPtr(), Game::GetNextSceneWeakPtr());
		return;
	}

	if (auto widgetRef = Cast<UMenuscene_StartGuide>(m_MenuSceneWidget))
	{
		if (auto ipText = Cast<SUITextComponent>(widgetRef->m_ipInputText))
		{
			ipText->SetContent(L"IP: " + std::wstring(InputIp.begin(), InputIp.end()));
		}
	}
}

void UMenuScene::EndGame()
{
	PostQuitMessage(0);
}
