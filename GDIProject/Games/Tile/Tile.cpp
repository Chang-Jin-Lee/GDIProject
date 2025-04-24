#include <Runtime/Core/FIleHelper.h>
#include "Tile.h"
#include "../Games.h"
#include "../Player/dGameState.h"

ATile::ATile()
{

}

ATile::~ATile()
{

}

void ATile::Initialize()
{
	__super::Initialize();

	AdGameState* gs = dynamic_cast<AdGameState*>(Game::GetGameState());
	if (gs)
	{
		int** cloneInfo = gs->m_TileCloneInfo;
		int cloneInfoRowSize = gs->m_TileCloneInfoRowSize;
		int cloneInfoColsize = gs->m_TileCloneInfoColsize;
		Gdiplus::Bitmap* baseImage = gs->m_baseTileImage;

		//mesh = baseBitMap;
		if (cloneInfo)
		{
			int Type = static_cast<int>(m_etileType);
			if (cloneInfo[Type] && cloneInfoColsize > 3)
			{
				int left = cloneInfo[Type][0], top = cloneInfo[Type][1], right = cloneInfo[Type][2], bottom = cloneInfo[Type][3];
				int width = right - left;
				int height = bottom - top;

				StaticMeshComponent->SetMeshSize(width, height);

				StaticMeshComponent->mesh = baseImage->Clone(left, top, width, height, PixelFormat32bppARGB);
			}
		}
	}
}

void ATile::Update()
{
	__super::Update();
}

void ATile::LoadData()
{
	__super::LoadData();
}
