#pragma once

#include "core/math/point2d.hpp"
#include "core/utils/random.hpp"

#include <vector>

namespace generation::pipeline
{
     std::vector<math::Point2Dd> samplePoints(math::RngEngine rngEngine,
                                              math::UnifDoubleDistribution& widthInterval,
                                              math::UnifDoubleDistribution& heightInterval, int numPoints);
}
