#pragma once

#include "resource.h"
#include <Runtime/Renderer/Renderer.h>
#include <Classes/Scene/Scene.h>
#include <Classes/GameFramework/GameState.h>

namespace Game
{
	void PreInitialize();		// GameState, GameInstance 같은 전역으로 관리될 객체를 생성하는 단계.
	void Initialize(HWND hwnd); // Scene에 종속된 모든 객체가 해당하는 게임 루프
	void LoadData();
	void Update();
	void Release(HWND hwnd);
	UScene* GetCurrentScene();
	UScene** GetCurrentScenePtr();
	UScene* GetNextScene();
	UScene** GetNextScenePtr();
	AGameStateBase* GetGameState();

	void ChangeScene();

	FVector2 GetLMouseClickPosition();
	FVector2 GetRMouseClickPosition();
	void SetLMouseClickPosition(const FVector2& rect);
	void SetRMouseClickPosition(const FVector2& rect);

	//void Render();
	//void Release();
	//ESceneState* GetCurrentSceneState();
	//void ChangeScene(const ESceneState& scene);
	//void GameExit();
}