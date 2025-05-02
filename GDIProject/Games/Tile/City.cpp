#include "City.h"
#include "../Games.h"
#include "Tile.h"
#include "../Player/TurnGameState.h"

void ACity::Initialize() 
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

		if (cloneInfo)
		{
			int Type = static_cast<int>(ETileType::Grassland);
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

void ACity::Update()
{
    ProductionPoints++;
}

std::shared_ptr<APlayerCharacter> ACity::ProduceUnit(EUnitType Type)
{
    auto unit = std::make_shared<APlayerCharacter>(Type);
    ProducedUnits.push_back(unit);
    ProductionPoints = 0;
    return unit;
}
