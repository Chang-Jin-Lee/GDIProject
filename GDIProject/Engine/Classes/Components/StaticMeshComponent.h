#pragma once
#include <string>
#include <memory>
#include "../Object.h"
template<typename T> class TVector2; using FVector2 = TVector2<float>;

namespace Gdiplus { class Bitmap; }

// staticMeshComponent는 LoadData로 에셋을 로드해주어야 함.
class UStaticMeshComponent : public UObject
{
public:
	UStaticMeshComponent();
	UStaticMeshComponent(std::wstring baseDir, std::wstring fileName);
	~UStaticMeshComponent();

	virtual void Initialize() override;
	virtual void Update() override;
	virtual void Release() override;

	void LoadData(std::wstring baseDir, std::wstring fileName);

	void SetMeshSize(float width, float height);
	void SetMeshScale(float widthRatio, float heightRatio);
    FVector2 GetMeshSize() const;
	Gdiplus::Bitmap* GetMesh() const;
	void SetMesh(Gdiplus::Bitmap* bitmap);

private:
	struct Impl;
	std::unique_ptr<Impl> pImpl;
};