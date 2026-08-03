#include "point_sampling.hpp"

#include "core/math/aabb.hpp"
#include "core/math/interval.hpp"
#include "core/utils/random.hpp"

#include <cstdint>


namespace generation::pipeline
{
    std::vector<math::Point2Dd> samplePoints(uint64_t seed, math::AABB bounds, int numPoints)
    {
        utils::RngEngine rngEngine(seed);
        const math::Interval<double> widthInterval(bounds.x_bounds.min, bounds.x_bounds.max);
        const math::Interval<double> heightInterval(bounds.y_bounds.min, bounds.y_bounds.max);

        std::vector<math::Point2Dd> points;
        points.reserve(numPoints);

        auto distX = utils::makeDistribution(widthInterval);
        auto distY = utils::makeDistribution(heightInterval);

        for (int i = 0; i < numPoints; ++i)
        {
            double x = distX(rngEngine);
            double y = distY(rngEngine);
            points.push_back({ x, y });
        }

        return points;
    }
}
