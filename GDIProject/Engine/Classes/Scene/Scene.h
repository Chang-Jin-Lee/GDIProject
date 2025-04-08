#pragma once

#include "../../Classes/Object.h"

class UScene : public UObject
{
public:
	UScene();
	~UScene();
	virtual void Initialize();
	virtual void Update();
	virtual void LoadData();
	virtual void Release();

	inline wchar_t* GetSceneName() { return SceneName; }
	inline void SetSceneName(wchar_t* value) { SceneName = value; }
	
private:
	wchar_t* SceneName;
};