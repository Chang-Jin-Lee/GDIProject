#pragma once
#include <windows.h>
#include <string>
#include <sstream>
#include <iomanip>
#include <memory>
#include "ObjectBase.h"
#include <iostream>
#include "../Experiment/SmartCast.h"

class UObject : public UObjectBase, public std::enable_shared_from_this<UObject>
{
public:
	UObject();
	virtual ~UObject();
	virtual void Initialize();
	virtual void Update();
	virtual void Release();

	std::wstring MakeUniqueName() {
		SYSTEMTIME st;
		GetLocalTime(&st);

		std::wstringstream wss;
		wss << L"_"
			<< std::setfill(L'0')
			<< std::setw(4) << st.wYear
			<< std::setw(2) << st.wMonth
			<< std::setw(2) << st.wDay << L"_"
			<< std::setw(2) << st.wHour
			<< std::setw(2) << st.wMinute
			<< std::setw(2) << st.wSecond
			<< L"_"
			<< std::setw(3) << st.wMilliseconds;

		return wss.str();
	}
	
	template<class TReturnType>
	std::weak_ptr<TReturnType> CreateDefaultSubobject(const std::wstring& SubobjectName, const int layer = 0)
	{
		std::shared_ptr<TReturnType> temp = std::make_shared<TReturnType>();
		std::wstring Name = SubobjectName + MakeUniqueName();
		if (std::shared_ptr<UObject> object = std::shared_ptr<UObject>(temp))	// ¸¸¾à ¾À¿¡¼­ »ý¼ºÇÒ¶§ À§Á¬ÀÌ ºÎÂøµÇ¾î ÀÖÀ¸¸é °Âµµ °ü¸®ÇØÁÜ.
		{
			object->SetEditorName(Name);
			object->RenderLayer = layer;
		}

		auto result = m_components.emplace(Name, temp);
		auto iter = result.first;
		return std::dynamic_pointer_cast<TReturnType>(iter->second);
	}

	void DestroyComponent(const std::wstring& name)
	{
		if(name.empty() == false)
			m_components.erase(name);
	}

	void DestroyAllComponent()
	{
		//for (auto& m_component : m_components)
		//{
		//	std::cout << "m_component.second.use_count() : " << m_component.second.use_count() << '\n';
		//	//if(m_component.second.use_count() > 0)
		//	m_component.second.reset();
		//}
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
