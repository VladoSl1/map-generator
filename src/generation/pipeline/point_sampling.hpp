#pragma once

#include "core/math/point2d.hpp"
#include "core/math/interval.hpp"
#include "core/utils/random.hpp"

#include <vector>

namespace generation::pipeline
{
    std::vector<math::Point2Dd> samplePoints(utils::RngEngine& rngEngine,
                                             const math::Interval<double>& widthInterval,
                                             const math::Interval<double>& heightInterval,
                                             int numPoints);
}
