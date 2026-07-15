#include "point_sampling.hpp"
#include "core/utils/random.hpp"

#include <vector>


namespace generation::pipeline
{
    std::vector<math::Point2Dd> samplePoints(math::RngEngine rngEngine,
                                              math::UnifDoubleDistribution& widthInterval,
                                              math::UnifDoubleDistribution& heightInterval, int numPoints)
    {
        std::vector<math::Point2Dd> points;
        points.reserve(numPoints);

        for (int i = 0; i < numPoints; ++i)
        {
            double x = math::getRandomDouble(rngEngine, widthInterval);
            double y = math::getRandomDouble(rngEngine, heightInterval);
            points.emplace_back(x, y);
        }

        return points;
    }
}

