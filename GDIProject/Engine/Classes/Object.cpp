#include "Object.h"
#include "Manager/ObjectManager.h"

UObject::UObject()
{
	Name = L"DefaultObject";
}

void UObject::Initialize()
{
	//UObjectManager<UObject>::Get();
}

void UObject::Update()
{
	if (bStatic == false)
	{
		for (auto component : m_components)
		{
			component.second->Update();
		}
	}
}

void UObject::Release()
{

}

void UObject::SetName(const std::wstring& value)
{
	Name = value;
	//size_t length = wcslen(value) + 1;
	//if (Name)
	//{
	//	size_t nameLength = wcslen(Name) + 1;
	//	size_t diff = nameLength - length;
	//	if (diff > 0)
	//	{
	//		memcpy_s(Name, length, value, length);
	//	}
	//	else
	//	{
	//		delete Name;
	//	}
	//}
	//Name = new wchar_t[length];
	//wmemset(Name, L'\0', length);
	//wcscpy_s(Name, length, value);
}

void UObject::SetEditorName(const std::wstring& value)
{
	InEditorName = value;
}