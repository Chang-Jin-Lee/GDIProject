#pragma once
#include "../Actor.h"
#include "../../UI/Widget.h"
#include "../Object.h"

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
	HUD,
	MAX
};

class GarbageUpdate
{
	virtual void DeleteNullObjects() = 0;
};

class UScene : public UObject, public GarbageUpdate, public std::enable_shared_from_this<UScene>
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
	void ChangeScene(std::shared_ptr<UScene>* curScene)
	{
		*curScene = std::make_shared<T>();
		(*curScene)->Initialize();
		(*curScene)->LoadData();
	}

	template<typename TReturnType>
	std::shared_ptr<TReturnType> NewObject(const std::wstring& NewobjectName, ESCENELAYER layer = ESCENELAYER::CHARACTER)
	{
		int intLayer = static_cast<int>(layer);
		std::shared_ptr<TReturnType> temp = std::make_shared<TReturnType>();
		if (std::shared_ptr<UObject> object = std::dynamic_pointer_cast<UObject>(temp))	// ¸¸¾à ¾À¿¡¼­ »ý¼ºÇÒ¶§ À§Á¬ÀÌ ºÎÂøµÇ¾î ÀÖÀ¸¸é °Âµµ °ü¸®ÇØÁÜ.
		{
			object->SetEditorName(NewobjectName);
			object->RenderLayer = intLayer;
			if (std::shared_ptr<AActor> actor = std::dynamic_pointer_cast<AActor>(object))
			{
				for (std::shared_ptr<UWidget>& widget : actor->attachedWidgets)
				{
					m_widgets[widget.get()->RenderLayer].insert({ widget->GetName(), widget });
				}
			}
		}
		m_objects[intLayer].insert({ NewobjectName, temp });
		return temp;
	}

	void Destroy(std::shared_ptr<AActor> actor)
	{
		for (std::shared_ptr<UWidget>& widget : actor->attachedWidgets)
		{
			Destroy<UWidget>(widget.get()->RenderLayer, widget->GetEditorName());
		}
		actor->DestroyAllComponent();
		Destroy<AActor>(actor->RenderLayer, actor->GetEditorName());
	}

	void Destroy(std::shared_ptr<UWidget> widget)
	{
		Destroy<UWidget>(widget->RenderLayer, widget->GetEditorName());
	}

	template<typename TargetType>
	void Destroy(const int& layer, const std::wstring& objectName)
	{
		if constexpr (std::is_same_v<TargetType, AActor>)
		{
			m_objects[layer].erase(objectName);
		}
		else if constexpr (std::is_same_v<TargetType, UWidget>)
		{
			m_widgets[layer].erase(objectName);
		}
	}

	template<typename TReturnType>
	std::shared_ptr<TReturnType> CreateWidget(const std::wstring& NewWidgetName, EUILAYER layer = EUILAYER::TEXT)
	{
		int intLayer = static_cast<int>(layer);
		std::shared_ptr<TReturnType> temp = std::make_shared<TReturnType>();
		if (std::shared_ptr<UObject> object = std::dynamic_pointer_cast<UObject>(temp))	// ¸¸¾à ¾À¿¡¼­ »ý¼ºÇÒ¶§ À§Á¬ÀÌ ºÎÂøµÇ¾î ÀÖÀ¸¸é °Âµµ °ü¸®ÇØÁÜ.
		{
			object->SetEditorName(NewWidgetName);
			object->RenderLayer = intLayer;
		}
		m_widgets[intLayer].insert({ NewWidgetName, temp });
		return temp;
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