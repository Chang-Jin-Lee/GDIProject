#pragma once
#include <functional>
#include <string>
#include <unordered_map>
#include <memory>
#include "../Camera/CameraActor.h"

class GameStateBase
{
public:
	GameStateBase();
	virtual ~GameStateBase();
	virtual void Initialize() = 0;
	virtual void PostInitialize() = 0;
	virtual void Release() = 0;

	using CreatorFunc = std::function<GameStateBase*()>;
	static void RegistGameState(const std::wstring& name, CreatorFunc func);
	static GameStateBase* CreateInstance();

	std::shared_ptr<ACameraActor> GetMainCamera()
	{
		return m_gMainCamera;
	}

public:
	std::shared_ptr<ACameraActor> m_gMainCamera;

private:
	static std::unordered_map<std::wstring, CreatorFunc>& Registry();
};