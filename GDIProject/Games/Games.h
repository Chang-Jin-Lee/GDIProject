#pragma once

#include "resource.h"
#include <Runtime/Renderer/Renderer.h>

namespace Game
{
	void Initialize(HWND hwnd);
	void LoadData();
	void Update();
	void Release(HWND hwnd);
	UScene* GetCurrentScene();
	UScene** GetCurrentScenePtr();
	UScene* GetNextScene();
	UScene** GetNextScenePtr();

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