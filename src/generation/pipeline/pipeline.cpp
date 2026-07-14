#include "pipeline.hpp"

#include "core/utils/random.hpp"
#include "generation/pipeline/delaunay_triangulation.hpp"
#include "generation/pipeline/point_sampling.hpp"
#include "generation/pipeline/voronoi_diagram.hpp"

#include "core/config.hpp"

#include "core/utils/debug.hpp"


namespace generation
{
    void GenerationPipeline::generate()
    {
        math::RngEngine rngEngine(m_worldSeed);
        math::UnifIntDistribution heightInterval(config::generation::INSIDE_PADDING,
                                                 m_height - config::generation::INSIDE_PADDING);
        math::UnifIntDistribution widthInterval(config::generation::INSIDE_PADDING,
                                                m_width - config::generation::INSIDE_PADDING);

        seeds = pipeline::samplePoints(rngEngine, widthInterval, heightInterval, config::generation::NUM_POINTS);

        // add boundary points for debugging purposes
        for (int i = 0; i < m_width; i += 100)
        {
            seeds.emplace_back(math::Point2Di{i, 0}.cast<double>());
            seeds.emplace_back(math::Point2Di{i, m_height}.cast<double>());
        }
        for (int i = 0; i < m_height; i += 100)
        {
            seeds.emplace_back(math::Point2Di{0, i}.cast<double>());
            seeds.emplace_back(math::Point2Di{m_width, i}.cast<double>());
        }

        generateFromPoints(std::move(seeds));
    }

    void GenerationPipeline::generateFromPoints(std::vector<math::Point2Dd> points)
    {
        log("pipeline generating");
        seeds = std::move(points);

        log("seeds moved");

        triangles = pipeline::triangulate(seeds);

        log("triangulation generated");

        voronoiDiagram.generate(seeds, triangles);

        log("voronoi diagram generated");

        voronoiVertices = voronoiDiagram.vertices;

        log("pipeline generated");

    }
}
