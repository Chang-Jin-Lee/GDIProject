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

	template<typename T>
	void ChangeScene(UScene** curScene)
	{
		*curScene = new T();
		(*curScene)->Initialize();
		(*curScene)->LoadData();
	}

	template<typename TReturnType>
	std::shared_ptr<TReturnType> NewObject(const std::wstring& NewobjectName)
	{
		if (m_objects.find(NewobjectName) != m_objects.end())
		{
			int i = 0;
			std::wstring originalNewobjectName = L"";
			
			auto it = NewobjectName.find(L"_");
			if (it == std::wstring::npos)
			{
				originalNewobjectName = NewobjectName;
			}
			else
			{
				originalNewobjectName = NewobjectName.substr(0,it);
				it++;
				std::wstring wstr = NewobjectName.substr(it);
				i = std::stoi(wstr);
			}
				
			while (m_objects.find(originalNewobjectName + L"_" + std::to_wstring(i)) != m_objects.end())
			{
				i++;
			}
			std::shared_ptr<TReturnType> temp = std::make_shared<TReturnType>();
			const_cast<std::wstring&>(NewobjectName) = originalNewobjectName + L"_" + std::to_wstring(i);
			m_objects.insert({ NewobjectName, temp });
			return temp;
		}
		else
		{
			std::shared_ptr<TReturnType> temp = std::make_shared<TReturnType>();
			m_objects.insert({ NewobjectName, temp });
			return temp;
		}
	}

	inline wchar_t* GetSceneName() { return m_sceneName; }
	inline void SetSceneName(wchar_t* value) { m_sceneName = value; }
	
private:
	wchar_t* m_sceneName;

protected:
	std::unordered_map<std::wstring, std::shared_ptr<UObject>> m_objects;
};