#include "Scene.h"
#include "../../Runtime/Renderer/Renderer.h"
#include "../Camera/CameraActor.h"
#include "../../UI/Widget.h"
#include "../Actor.h"
#include "../../Math/Math.h"

UScene::UScene()
{
	m_objects.assign(DEFAULT_SCENELAYER_SIZE, std::unordered_map<std::wstring, std::shared_ptr<AActor>>());
	m_widgets.assign(DEFAULT_UILAYER_SIZE, std::unordered_map<std::wstring, std::shared_ptr<UWidget>>());
}

UScene::~UScene()
{
	for (auto m_object : m_objects)
	{
		for (auto object : m_object)
		{
			object.second.reset();
		}
		m_object.clear();
	}
	m_objects.clear();

	for (auto m_widget : m_widgets)
	{
		for (auto widget : m_widget)
		{
			widget.second.reset();
		}
		m_widget.clear();
	}
	m_widgets.clear();
}

void UScene::Initialize()
{
	__super::Initialize();
}

void UScene::Update()
{
	__super::Update();
	Renderer::ClearRenderObjects();
	for (auto& objectMap : m_objects)
	{
		for (auto& objectPair : objectMap)
		{
			if (objectPair.second)
			{
				objectPair.second->Update();

				if (auto cameraRef = Renderer::GetMainCamera().lock())
				{
					if (objectPair.second.get()->GetActorLocation().x < cameraRef->GetCameraLocation().x - Renderer::GetResolution().x * 0.2f ||
						objectPair.second.get()->GetActorLocation().x > cameraRef->GetCameraLocation().x + Renderer::GetResolution().x * 1.2f ||
						objectPair.second.get()->GetActorLocation().y < cameraRef->GetCameraLocation().y - Renderer::GetResolution().x * 0.2f ||
						objectPair.second.get()->GetActorLocation().y > cameraRef->GetCameraLocation().y + Renderer::GetResolution().y * 1.2f
						)
						continue;
				}
				
				Renderer::SetRenderObject(objectPair.second);
			}
			else
			{
				bEraseOjbect = true;
			}
		}
	}

	for (auto& widgetMap : m_widgets)
	{
		for (auto& widget : widgetMap)
		{
			if (widget.second)
			{
				if (widget.second.get()->bVisible)
				{
					Renderer::SetRenderObject(widget.second);
				}
			}
			else
			{
				bEraseOjbect = true;
			}
		}
	}

}

void UScene::LoadData()
{

}

void UScene::Release()
{
	__super::Release();
	for (auto& m_object : m_objects)
	{
		for (auto& object : m_object)
		{
			std::cout << "이전 m_component.second.use_count() : " << object.second.use_count() << '\n';
			object.second.reset();
			//object.second.reset();
		}
		m_object.clear();
	}
	m_objects.clear();
	std::vector<std::unordered_map<std::wstring, std::shared_ptr<AActor>>>().swap(m_objects);

	for (auto& m_widget : m_widgets)
	{
		for (auto& widget : m_widget)
		{
			std::cout << "이전 m_component.second.use_count() : " << widget.second.use_count() << '\n';
			widget.second.reset();
			//widget.second.reset();
		}
		m_widget.clear();
	}
	m_widgets.clear();
	std::vector<std::unordered_map<std::wstring, std::shared_ptr<UWidget>>>().swap(m_widgets);
}
void UScene::Destroy(std::weak_ptr<AActor> actor)
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

void UScene::Destroy(std::weak_ptr<UWidget> widget)
{
    if (auto shared = widget.lock())
    {
        Destroy<UWidget>(shared->RenderLayer, shared->GetEditorName());
    }
}

void UScene::RegisterAttachedWidgets(const std::shared_ptr<AActor>& actor,
    std::vector<std::unordered_map<std::wstring, std::shared_ptr<UWidget>>>& widgets)
{
    for (const auto& widgetWeak : actor->attachedWidgets)
    {
        if (auto widgetShared = widgetWeak.lock())
        {
            widgets[widgetShared->RenderLayer].emplace(widgetShared->GetEditorName(), std::move(widgetShared));
        }
    }
}

void UScene::DeleteNullObjects()
{
	if (!bEraseOjbect)
		return;

	// AActor 삭제
	for (auto& map : m_objects)
	{
		for (auto it = map.begin(); it != map.end(); )
		{
			if (it->second == nullptr)
			{
				it = map.erase(it); // erase가 다음 iterator를 반환합니다
			}
			else
			{
				++it;
			}
		}
	}

	// UWidget 삭제
	for (auto& map : m_widgets)
	{
		for (auto it = map.begin(); it != map.end(); )
		{
			if (it->second == nullptr)
			{
				it = map.erase(it);
			}
			else
			{
				++it;
			}
		}
	}
	

	bEraseOjbect = false;
}

