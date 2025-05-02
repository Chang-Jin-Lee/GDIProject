#pragma once
#include "ObjectBase.h"
#include <iostream>

class UObject : public UObjectBase
{
public:
	UObject();
	virtual ~UObject();
	virtual void Initialize();
	virtual void Update();
	virtual void Release();
	
	template<class TReturnType>
	std::shared_ptr<TReturnType> CreateDefaultSubobject(const std::wstring& SubobjectName, const int layer = 0)
	{
		std::shared_ptr<TReturnType> temp = std::make_shared<TReturnType>();
		if (std::shared_ptr<UObject> object = std::dynamic_pointer_cast<UObject>(temp))	// 만약 씬에서 생성할때 위젯이 부착되어 있으면 걔도 관리해줌.
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
			std::cout << "이전 m_component.second.use_count() : " << m_component.second.use_count() << '\n';
			while (m_component.second.use_count() > 0)
			{
				m_component.second.reset();
			}
			std::cout << "이후 m_component.second.use_count() : " << m_component.second.use_count() << '\n';
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
