#include "point_sampling.hpp"

namespace generation::pipeline
{
    std::vector<math::Point2Dd> samplePoints(utils::RngEngine& rngEngine,
                                             const math::Interval<double>& widthInterval,
                                             const math::Interval<double>& heightInterval,
                                             int numPoints)
    {
        std::vector<math::Point2Dd> points;
        points.resize(numPoints);

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
