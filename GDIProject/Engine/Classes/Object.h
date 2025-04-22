#pragma once
#include <wchar.h>
#include <unordered_map>
#include <string>

class UObject
{
public:
	UObject();
	virtual ~UObject() {}
	virtual void Initialize();
	virtual void Update();
	virtual void Release();
	
	template<class TReturnType>
	TReturnType* CreateDefaultSubobject(std::wstring SubobjectName)
	{
		if (m_Components.find(SubobjectName) != m_Components.end())
		{
			int i = 0;
			std::wstring nextSubobjectName = SubobjectName + L"_" + std::to_wstring(i++);
			while (m_Components.find(nextSubobjectName) != m_Components.end())
			{
				nextSubobjectName = SubobjectName + L"_" + std::to_wstring(i++);
			}
			TReturnType* temp = new TReturnType();
			m_Components.insert({ nextSubobjectName, temp });
			return temp;
		}
		else
		{
			TReturnType* temp = new TReturnType();
			m_Components.insert({ SubobjectName, temp });
			return temp;
		}
	}
	
	wchar_t* GetName() { return Name; }
	void SetName(const wchar_t* value);

private:
	std::unordered_map<std::wstring, UObject*> m_Components;
	wchar_t* Name;
};
