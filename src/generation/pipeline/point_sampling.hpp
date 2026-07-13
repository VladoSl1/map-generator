#pragma once

#include "core/math/point2d.hpp"
#include "core/utils/random.hpp"

#include <vector>

namespace generation::pipeline
{
     std::vector<math::Point2Di> samplePoints(math::RngEngine rngEngine,
                                              math::UnifIntDistribution& widthInterval,
                                              math::UnifIntDistribution& heightInterval, int numPoints);
}
