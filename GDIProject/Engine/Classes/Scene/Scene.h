#pragma once
#include "../Actor.h"

#define DEFAULT_LAYER_SIZE 3

enum class ELAYER
{
	GROUND,
	CHARACTER,
	UI,
	MAX
};

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

	bool HasObjectName(const std::wstring& NewobjectName)
	{
		for (auto objectPair : m_objects)
		{
			if (objectPair.find(NewobjectName) != objectPair.end())
			{
				return true;
			}
		}
		return false;
	}

	void SetCountDuplicateObjectName(const std::wstring& objectName, int& index)
	{
		for (auto objectPair : m_objects)
		{
			while (objectPair.find(objectName + L"_" + std::to_wstring(index)) != objectPair.end())
			{
				index++;
			}
		}
	}

	template<typename TReturnType>
	std::shared_ptr<TReturnType> NewObject(const std::wstring& NewobjectName, ELAYER layer = ELAYER::CHARACTER)
	{
		int intLayer = static_cast<int>(layer);
		if (HasObjectName(NewobjectName))
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
				
			SetCountDuplicateObjectName(originalNewobjectName, i);
			std::shared_ptr<TReturnType> temp = std::make_shared<TReturnType>();
			const_cast<std::wstring&>(NewobjectName) = originalNewobjectName + L"_" + std::to_wstring(i);
			m_objects[intLayer].insert({NewobjectName, temp});
			return temp;
		}
		else
		{
			std::shared_ptr<TReturnType> temp = std::make_shared<TReturnType>();
			m_objects[intLayer].insert({ NewobjectName, temp });
			return temp;
		}
	}

	inline wchar_t* GetSceneName() { return m_sceneName; }
	inline void SetSceneName(wchar_t* value) { m_sceneName = value; }
	
private:
	wchar_t* m_sceneName;

protected:
	std::vector<std::unordered_map<std::wstring, std::shared_ptr<AActor>>> m_objects;
};