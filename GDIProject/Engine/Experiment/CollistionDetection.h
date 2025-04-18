#pragma once
#include "AABBBox.h"

namespace Experiment
{
	namespace FCollisionDetector
	{
		bool AABBCollisionCheck(const FAABBBox& boundA, const FAABBBox& boundB)
		{
			if ((boundA.m_maxVector.y < boundB.m_minVector.y) || (boundA.m_minVector.y > boundB.m_maxVector.y)
				|| (boundA.m_minVector.x > boundB.m_maxVector.x) || (boundA.m_maxVector.x < boundB.m_minVector.x))
			{
				return false;
			}
			return true;
		}
	}
}
