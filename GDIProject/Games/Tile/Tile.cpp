#include <Runtime/Core/FIleHelper.h>
#include "Tile.h"
#include "../Games.h"
#include "../Player/TurnGameState.h"
#include "../Scene/PlayScene.h"

ATile::ATile()
{
	bStatic = true;
}

ATile::~ATile()
{

}

void ATile::Initialize()
{
	__super::Initialize();

	bStatic = true;

	TurnGameState* gs = dynamic_cast<TurnGameState*>(Game::GetGameState());
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
				//InitialTileSize = FVector2(width, height);
				StaticMeshComponent->mesh = baseImage->Clone(left, top, width, height, PixelFormat32bppARGB);
			}
		}
	}
	//StaticMeshComponent->SetMeshSize(InitialTileSize.x, InitialTileSize.y);
}

void ATile::Update()
{
	__super::Update();
}

void ATile::LoadData()
{
	__super::LoadData();
}

FVector2 ATile::GetTilePositionAtIndex(int row, int col)
{
	if (row < 0 || col < 0 || row >= TILE_ROW_SIZE || col >= TILE_COL_SIZE) return FVector2(0,0);
	if (row % 2 == 0)
	{
		return FVector2(col * initialTileSizeX + initialTileSizeX / 2, row * ((initialTileSizeY * 2) / 3) + (initialTileSizeY / 2));
	}
	else
	{
		return FVector2(col * initialTileSizeX + initialTileSizeX, row * ((initialTileSizeY * 2) / 3) + (initialTileSizeY / 2));
	}
}

FVector2 ATile::GetIndexAtPosition(FVector2 position)
{
	if (position.x < 0 || position.y < 0) return FVector2(0, 0);
	int row = (int)position.y / (int)((initialTileSizeY * 2) / 3);
	int col = 0;
	if (row % 2 == 0)
	{
		col = (int)position.x / initialTileSizeX;
	}
	else
	{
		col = ((int)position.x - initialTileSizeX / 2) / initialTileSizeX;
	}
	
	return FVector2(row, col);
}

std::wstring ATile::GetTileName()
{
	switch (m_etileType)
	{
	case ETileType::Desert:
		return L"사막";
	case ETileType::Grassland:
		return L"초원";
	case ETileType::Hills:
		return L"언덕";
	case ETileType::Plain:
		return L"평지";
	case ETileType::Mountain:
		return L"산";
	case ETileType::MAX:
		return L"MAX";
	default:
		return L"default";
	}

}
