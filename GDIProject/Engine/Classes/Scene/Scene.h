#pragma once

#include "../../Classes/Object.h"
#include <vector>
#include <memory>

class UScene : public UObject
{
public:
	UScene();
	~UScene();
	virtual void Initialize();
	virtual void Update();
	virtual void LoadData();
	virtual void Release();

	template<typename T>
	void ChangeScene(UScene** curScene)
	{
		*curScene = new T();
		(*curScene)->Initialize();
		(*curScene)->LoadData();
	}

	inline wchar_t* GetSceneName() { return m_sceneName; }
	inline void SetSceneName(wchar_t* value) { m_sceneName = value; }
	
private:
	wchar_t* m_sceneName;

protected:
	std::vector<std::shared_ptr<UObject>> m_objects;
};