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
	void ChangeScene(std::shared_ptr<UScene>& nextSceneShared, std::weak_ptr<UScene>& nextSceneWeak)
	{
		nextSceneShared = std::make_shared<T>();
		nextSceneShared->Initialize();
		nextSceneShared->LoadData();
		nextSceneWeak = nextSceneShared;
	}

	template<typename TReturnType>
	std::weak_ptr<TReturnType> NewObject(const std::wstring& NewobjectName, ESCENELAYER layer = ESCENELAYER::CHARACTER)
	{
		int intLayer = static_cast<int>(layer);
		std::shared_ptr<TReturnType> temp = std::make_shared<TReturnType>();
		if (std::shared_ptr<UObject> object = std::dynamic_pointer_cast<UObject>(temp))	// ¸¸¾à ¾À¿¡¼­ »ý¼ºÇÒ¶§ À§Á¬ÀÌ ºÎÂøµÇ¾î ÀÖÀ¸¸é °Âµµ °ü¸®ÇØÁÜ.
		{
			object->SetEditorName(NewobjectName);
			object->RenderLayer = intLayer;

			if (std::shared_ptr<AActor> actor = std::dynamic_pointer_cast<AActor>(object))
			{
				for (const auto& widgetWeak : actor->attachedWidgets)
				{
					if (auto widgetShared = widgetWeak.lock())
					{
						m_widgets[widgetShared->RenderLayer].emplace(widgetShared->GetName(), std::move(widgetShared));
					}
				}
			}
		}

		auto result = m_objects[intLayer].emplace(NewobjectName, std::move(temp));
		auto iter = result.first;
		return std::dynamic_pointer_cast<TReturnType>(iter->second);
	}

	void Destroy(std::weak_ptr<AActor> actor)
	{
		if (auto actorRef = actor.lock())
		{
			for (std::weak_ptr<UWidget> widget : actorRef->attachedWidgets)
			{
				if (auto widgetRef = widget.lock())
				{
					Destroy<UWidget>(widgetRef->RenderLayer, widgetRef->GetEditorName());
				}
			}
			actorRef->DestroyAllComponent();
			Destroy<AActor>(actorRef->RenderLayer, actorRef->GetEditorName());
		}
	}

	void Destroy(std::weak_ptr<UWidget> widget)
	{
		if (auto shared = widget.lock())
		{
			Destroy<UWidget>(shared->RenderLayer, shared->GetEditorName());
		}
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
	std::weak_ptr<TReturnType> CreateWidget(const std::wstring& NewWidgetName, EUILAYER layer = EUILAYER::TEXT)
	{
		int intLayer = static_cast<int>(layer);
		std::shared_ptr<TReturnType> temp = std::make_shared<TReturnType>();
		if (std::shared_ptr<UObject> object = std::dynamic_pointer_cast<UObject>(temp))	// ¸¸¾à ¾À¿¡¼­ »ý¼ºÇÒ¶§ À§Á¬ÀÌ ºÎÂøµÇ¾î ÀÖÀ¸¸é °Âµµ °ü¸®ÇØÁÜ.
		{
			object->SetEditorName(NewWidgetName);
			object->RenderLayer = intLayer;
		}

		auto result = m_widgets[intLayer].emplace(NewWidgetName, std::move(temp));
		auto iter = result.first;
		return std::dynamic_pointer_cast<TReturnType>(iter->second);
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