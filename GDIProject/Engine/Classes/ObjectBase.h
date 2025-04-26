#pragma once
#include <wchar.h>
#include <unordered_map>
#include <string>
#include <memory>
#include <functional>

class UObjectBase
{
public:
	virtual void Initialize() = 0;
	virtual void Update() = 0;
	virtual void Release() = 0;


	std::function<void()> FVoidDelegate;
	void SetVoidDelegate(const std::function<void()>& f)
	{
		FVoidDelegate = std::move(f);
	}
};