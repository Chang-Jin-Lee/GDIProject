#pragma once

#include <wchar.h>

class UObject
{
public:
	UObject() : Name(nullptr){}
	virtual ~UObject() {}
	virtual void Initialize() = 0;
	virtual void Update() = 0;
	virtual void Release() = 0;
	
	wchar_t* GetName() { return Name; }
	void SetName(wchar_t* value);
private:
	wchar_t* Name;
};
