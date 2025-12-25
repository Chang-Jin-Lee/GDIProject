#include "CollistionDetection.h"
#include "AABBBox.h"

namespace Experiment
{
    namespace FCollisionDetector
    {
        bool AABBCollisionCheck(const FAABBBox& boundA, const FAABBBox& boundB)
        {
            const auto aMin = boundA.GetMinVector();
            const auto aMax = boundA.GetMaxVector();
            const auto bMin = boundB.GetMinVector();
            const auto bMax = boundB.GetMaxVector();

            if ((aMax.y < bMin.y) || (aMin.y > bMax.y)
                || (aMin.x > bMax.x) || (aMax.x < bMin.x))
            {
                return false;
            }
            return true;
        }
    }
}


