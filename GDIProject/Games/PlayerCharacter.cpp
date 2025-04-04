#include "PlayerCharacter.h"

APlayerCharacter::APlayerCharacter()
{
	for(int i = 0 ; i < static_cast<int>(AnimationState::Max); i++)
		animationComponent[i] = new UAnimationComponent();
	animationstate = AnimationState::Idle;
}

APlayerCharacter::~APlayerCharacter()
{
}

void APlayerCharacter::Initialize(int rowSize, int colSize)
{
	for (int i = 0; i < static_cast<int>(AnimationState::Max); i++)
		animationComponent[i]->Initialize(rowSize, colSize);
}

void APlayerCharacter::LoadData(Gdiplus::Bitmap* baseImage, int** cloneInfo, int rowSize, int colSize, AnimationState state, int pixelformat)
{
	animationComponent[static_cast<int>(state)]->LoadData(baseImage, cloneInfo, rowSize, colSize, pixelformat);
}
