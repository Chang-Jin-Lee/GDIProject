#pragma once

class UObject
{
public:
	UObject() {}
	virtual ~UObject() {}
	virtual void Initialize() = 0;
	virtual void Update() = 0;
	virtual void Release() = 0;
};
