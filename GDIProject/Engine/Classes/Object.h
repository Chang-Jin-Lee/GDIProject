#pragma once
#include "ObjectBase.h"

class UObject : public UObjectBase
{
public:
	UObject();
	virtual ~UObject() {}
	virtual void Initialize();
	virtual void Update();
	virtual void Release();
	
	template<class TReturnType>
	std::shared_ptr<TReturnType> CreateDefaultSubobject(const std::wstring& SubobjectName, const int layer = 0)
	{
		std::shared_ptr<TReturnType> temp = std::make_shared<TReturnType>();
		if (std::shared_ptr<UObject> object = std::dynamic_pointer_cast<UObject>(temp))	// ¸¸¾à ¾À¿¡¼­ »ý¼ºÇÒ¶§ À§Á¬ÀÌ ºÎÂøµÇ¾î ÀÖÀ¸¸é °Âµµ °ü¸®ÇØÁÜ.
		{
			object->SetEditorName(SubobjectName);
			object->RenderLayer = layer;
		}
		m_components.insert(std::make_pair(SubobjectName, temp));
		return temp;
	}

	void DestroyComponent(const std::wstring& name)
	{
		if(name.empty() == false)
			m_components.erase(name);
	}

	void DestroyAllComponent()
	{
		for (auto& m_component : m_components)
		{
			m_component.second.reset();
		}
		m_components.clear();
	}
	
	std::wstring GetName() { return Name; }
	void SetName(const std::wstring& value);

	std::wstring GetEditorName() { return InEditorName; }
	void SetEditorName(const std::wstring& value);
public:
	bool bStatic = false;
	int RenderLayer = 0;
private:
	std::unordered_map<std::wstring, std::shared_ptr<UObject>> m_components;
	std::wstring Name;
	std::wstring InEditorName;
};
