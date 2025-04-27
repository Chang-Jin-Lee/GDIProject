#include "City.h"
#include "../Player/TurnGameState.h"
#include "../Games.h"

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

		//mesh = baseBitMap;
		if (cloneInfo)
		{
			int Type = static_cast<int>(ETileType::Capital);
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

void ACity::Update()
{
    ProductionPoints++;
}

std::shared_ptr<AUnit> ACity::ProduceUnit(EUnitType Type)
{
    auto unit = std::make_shared<AUnit>(Type);
    ProducedUnits.push_back(unit);
    ProductionPoints = 0;
    return unit;
}
