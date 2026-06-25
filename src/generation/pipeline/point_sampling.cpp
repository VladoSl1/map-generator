#include "point_sampling.hpp"
#include "core/utils/random.hpp"


#include <vector>

namespace generation::pipeline
{
    void samplePoints(std::vector<math::Point2D> *points, math::RngEngine *rngEngine,
                      math::UnifIntDistribution widthInterval,
                      math::UnifIntDistribution heightInterval, int numPoints)
    {
        points->clear();
        points->reserve(numPoints);

        for (int i = 0; i < numPoints; ++i)
        {
            int x = math::getRandomInt(*rngEngine, widthInterval);
            int y = math::getRandomInt(*rngEngine, heightInterval);
            points->emplace_back(x, y);
        }
    }
}

