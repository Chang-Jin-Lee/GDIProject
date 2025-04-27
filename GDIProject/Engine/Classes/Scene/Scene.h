#pragma once
#include "../Actor.h"
#include "../../UI/Widget.h"

#define DEFAULT_SCENELAYER_SIZE 3
#define DEFAULT_UILAYER_SIZE 3

enum class ESCENELAYER
{
	GROUND,
	CHARACTER,
	MAX
};

enum class EUILAYER
{
	BUTTON,
	TEXT,
	MAX
};

class GarbageUpdate
{
	virtual void DeleteNullObjects() = 0;
};

class UScene : public UObject, public GarbageUpdate
{
public:
	UScene();
	~UScene();
	virtual void Initialize();
	virtual void Update();
	virtual void LoadData();
	virtual void Release();

	virtual void DeleteNullObjects();

	template<typename T>
	void ChangeScene(UScene** curScene)
	{
		*curScene = new T();
		(*curScene)->Initialize();
		(*curScene)->LoadData();
	}

	template<typename T>
	bool HasObjectName(const std::wstring& NewobjectName, std::vector<std::unordered_map<std::wstring, std::shared_ptr<T>>> objects)
	{
		for (auto objectPair : objects)
		{
			if (objectPair.find(NewobjectName) != objectPair.end())
			{
				return true;
			}
		}
		return false;
	}

	template<typename T>
	void SetCountDuplicateObjectName(const std::wstring& objectName, std::vector<std::unordered_map<std::wstring, std::shared_ptr<T>>> objects, int& index)
	{
		for (auto objectPair : objects)
		{
			while (objectPair.find(objectName + L"_" + std::to_wstring(index)) != objectPair.end())
			{
				index++;
			}
		}
	}

	template<typename TReturnType>
	std::shared_ptr<TReturnType> NewObject(const std::wstring& NewobjectName, ESCENELAYER layer = ESCENELAYER::CHARACTER)
	{
		int intLayer = static_cast<int>(layer);
		std::shared_ptr<TReturnType> temp = std::make_shared<TReturnType>();
		if (std::shared_ptr<AActor> actor = std::dynamic_pointer_cast<AActor>(temp))	// 만약 씬에서 생성할때 위젯이 부착되어 있으면 걔도 관리해줌.
		{
			for (std::shared_ptr<UWidget>& widget : actor->attachedWidgets)
			{
				m_widgets[widget.get()->RenderLayer].insert({ widget->GetName(), widget });
			}
		}
		m_objects[intLayer].insert({ NewobjectName, temp });
		return temp;

		if (HasObjectName<AActor>(NewobjectName, m_objects))
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
				
			SetCountDuplicateObjectName<AActor>(originalNewobjectName, m_objects, i);
			const_cast<std::wstring&>(NewobjectName) = originalNewobjectName + L"_" + std::to_wstring(i);

			std::shared_ptr<TReturnType> temp = std::make_shared<TReturnType>();
			if (std::shared_ptr<AActor> actor = std::dynamic_pointer_cast<AActor>(temp))	// 만약 씬에서 생성할때 위젯이 부착되어 있으면 걔도 관리해줌.
			{
				for (std::shared_ptr<UWidget>& widget : actor->attachedWidgets)
				{
					m_widgets[widget.get()->RenderLayer].insert({widget->GetName(), widget});
				}
			}
			m_objects[intLayer].insert({NewobjectName, temp});
			return temp;
		}
		else
		{
			std::shared_ptr<TReturnType> temp = std::make_shared<TReturnType>();
			if (std::shared_ptr<AActor> actor = std::dynamic_pointer_cast<AActor>(temp))	// 만약 씬에서 생성할때 위젯이 부착되어 있으면 걔도 관리해줌.
			{
				for (std::shared_ptr<UWidget>& widget : actor->attachedWidgets)
				{
					m_widgets[widget.get()->RenderLayer].insert({ widget->GetName(), widget });
				}
			}
			m_objects[intLayer].insert({ NewobjectName, temp });
			return temp;
		}
	}

	template<typename TReturnType>
	std::shared_ptr<TReturnType> CreateWidget(const std::wstring& NewWidgetName, EUILAYER layer = EUILAYER::TEXT)
	{
		int intLayer = static_cast<int>(layer);
		std::shared_ptr<TReturnType> temp = std::make_shared<TReturnType>();
		m_widgets[intLayer].insert({ NewWidgetName, temp });
		return temp;

		if (HasObjectName<UWidget>(NewWidgetName, m_widgets))
		{
			int i = 0;
			std::wstring originalNewobjectName = L"";

			auto it = NewWidgetName.find(L"_");
			if (it == std::wstring::npos)
			{
				originalNewobjectName = NewWidgetName;
			}
			else
			{
				originalNewobjectName = NewWidgetName.substr(0, it);
				it++;
				std::wstring wstr = NewWidgetName.substr(it);
				i = std::stoi(wstr);
			}

			SetCountDuplicateObjectName<UWidget>(originalNewobjectName, m_widgets, i);
			std::shared_ptr<TReturnType> temp = std::make_shared<TReturnType>();
			const_cast<std::wstring&>(NewWidgetName) = originalNewobjectName + L"_" + std::to_wstring(i);
			m_widgets[intLayer].insert({NewWidgetName, temp});
			return temp;
		}
		else
		{
			std::shared_ptr<TReturnType> temp = std::make_shared<TReturnType>();
			m_widgets[intLayer].insert({NewWidgetName, temp});
			return temp;
		}
	}

	inline wchar_t* GetSceneName() { return m_sceneName; }
	inline void SetSceneName(wchar_t* value) { m_sceneName = value; }

	inline std::vector<std::unordered_map<std::wstring, std::shared_ptr<UWidget>>>& GetWidgets() { return m_widgets; }
	
private:
	wchar_t* m_sceneName;

protected:
	std::vector<std::unordered_map<std::wstring, std::shared_ptr<AActor>>> m_objects;
	std::vector<std::unordered_map<std::wstring, std::shared_ptr<UWidget>>> m_widgets;

public:
	bool bEraseOjbect = false;
};