#pragma once

#include <wchar.h>

class UObject
{
public:
	UObject() : Name(nullptr){}
	virtual ~UObject() {}
	virtual void Initialize();
	virtual void Update();
	virtual void Release();
	
	wchar_t* GetName() { return Name; }
	void SetName(wchar_t* value);
private:
	wchar_t* Name;
};
