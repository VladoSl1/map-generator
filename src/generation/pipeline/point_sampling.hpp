#pragma once

#include "core/math/math.hpp"
#include "core/utils/random.hpp"

#include <vector>

namespace generation::pipeline
{
    void samplePoints(std::vector<math::Point2D> *points, math::RngEngine *rngEngine,
                      math::UnifIntDistribution widthInterval,
                      math::UnifIntDistribution heightInterval, int numPoints);
}
