#pragma once

template <typename T>
class UObjectManager {
public:
	UObjectManager()
	{
		assert(s_instance == nullptr && "Singleton instance already created!");
		s_instance = static_cast<T*>(this);
	}

	virtual ~UObjectManager() = default;

	// 복사 및 이동 금지
	UObjectManager(const UObjectManager&) = delete;
	UObjectManager& operator=(const UObjectManager&) = delete;
	UObjectManager(UObjectManager&&) = delete;
	UObjectManager& operator=(UObjectManager&&) = delete;

	static T& Get()
	{
		assert(s_instance != nullptr && "Singleton instance not created!");
		return *s_instance;
	}
private:
	static T* s_instance;
};

template <typename T>
T* UObjectManager<T>::s_instance = nullptr;