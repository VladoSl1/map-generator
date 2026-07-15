#include "pipeline.hpp"

#include "core/math/point2d.hpp"
#include "core/utils/random.hpp"
#include "generation/pipeline/delaunay_triangulation.hpp"
#include "generation/pipeline/point_sampling.hpp"
#include "generation/pipeline/voronoi_diagram.hpp"

#include "core/config.hpp"

#include "core/utils/debug.hpp"
#include <vector>


namespace generation::pipeline
{
    VoronoiDiagram generate(uint64_t seed, int width, int height)
    {
        math::RngEngine rngEngine(seed);
        math::UnifDoubleDistribution heightInterval(0, height);
        math::UnifDoubleDistribution widthInterval(0, width);

        auto seedPoints = pipeline::samplePoints(rngEngine, widthInterval, heightInterval, config::generation::NUM_POINTS);

        // add boundary points for debugging purposes
        // for (int i = 0; i < m_width; i += 100)
        // {
        //     seeds.emplace_back(math::Point2Di{i, 0}.cast<double>());
        //     seeds.emplace_back(math::Point2Di{i, m_height}.cast<double>());
        // }
        // for (int i = 0; i < m_height; i += 100)
        // {
        //     seeds.emplace_back(math::Point2Di{0, i}.cast<double>());
        //     seeds.emplace_back(math::Point2Di{m_width, i}.cast<double>());
        // }

        return generateFromPoints(std::move(seedPoints));
    }

    VoronoiDiagram generateFromPoints(std::vector<math::Point2Dd> seedPoints)
    {
        auto triangles = pipeline::triangulate(seedPoints);

        VoronoiDiagram voronoiDiagram;

        voronoiDiagram.generate(seedPoints, triangles);

        return voronoiDiagram;
    }
}
