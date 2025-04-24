#pragma once
#include <functional>
#include <string>
#include <unordered_map>
#include <memory>
#include "../Camera/CameraActor.h"

class AGameStateBase
{
public:
	AGameStateBase();
	virtual ~AGameStateBase();
	virtual void Initialize() = 0;
	virtual void Release() = 0;

	using CreatorFunc = std::function<AGameStateBase*()>;
	static void RegistGameState(const std::wstring& name, CreatorFunc func);
	static AGameStateBase* CreateInstance();

	std::shared_ptr<ACameraActor> GetMainCamera()
	{
		return m_gMainCamera;
	}

public:
	std::shared_ptr<ACameraActor> m_gMainCamera;

private:
	static std::unordered_map<std::wstring, CreatorFunc>& Registry();
};