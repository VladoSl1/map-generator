#include "point_sampling.hpp"
#include "core/utils/random.hpp"

#include <vector>


namespace generation::pipeline
{
     std::vector<math::Point2Di> samplePoints(math::RngEngine rngEngine,
                                              math::UnifIntDistribution& widthInterval,
                                              math::UnifIntDistribution& heightInterval, int numPoints)
    {
        std::vector<math::Point2Di> points;
        points.reserve(numPoints);

        for (int i = 0; i < numPoints; ++i)
        {
            int x = math::getRandomInt(rngEngine, widthInterval);
            int y = math::getRandomInt(rngEngine, heightInterval);
            points.emplace_back(x, y);
        }

        return points;
    }
}

