#pragma once
class FAABBBox;

namespace Experiment
{
	namespace FCollisionDetector
	{
        bool AABBCollisionCheck(const FAABBBox& boundA, const FAABBBox& boundB);
	}
}
