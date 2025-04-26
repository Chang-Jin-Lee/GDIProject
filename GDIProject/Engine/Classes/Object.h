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
	std::shared_ptr<TReturnType> CreateDefaultSubobject(const std::wstring& SubobjectName)
	{
		if (m_components.find(SubobjectName) != m_components.end())
		{
			int i = 0;
			std::wstring nextSubobjectName = SubobjectName + L"_" + std::to_wstring(i++);
			while (m_components.find(nextSubobjectName) != m_components.end())
			{
				nextSubobjectName = SubobjectName + L"_" + std::to_wstring(i++);
			}
			std::shared_ptr<TReturnType> temp = std::make_shared<TReturnType>();
			m_components.insert(std::make_pair(nextSubobjectName, temp));
			return temp;
		}
		else
		{
			std::shared_ptr<TReturnType> temp = std::make_shared<TReturnType>();
			m_components.insert(std::make_pair(SubobjectName, temp));
			return temp;
		}
	}
	
	wchar_t* GetName() { return Name; }
	void SetName(const wchar_t* value);

public:
	bool bStatic = false;
private:
	std::unordered_map<std::wstring, std::shared_ptr<UObject>> m_components;
	wchar_t* Name;
};
