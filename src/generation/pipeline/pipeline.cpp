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

        points = pipeline::samplePoints(rngEngine, widthInterval, heightInterval, config::generation::NUM_POINTS);

        triangles = pipeline::triangulate(points);

        pipeline::sortTriangles(triangles);

        voronoiDiagram = pipeline::generateVoronoi(points, triangles);

        voronoiVertices = voronoiDiagram.vertices;

        std::cout << voronoiDiagram.vertices.size() << " voronoi vertices generated" << std::endl;
        for (const auto& edge : voronoiDiagram.edges)
        {
            std::cout << edge[0] << " -> " << edge[1] << std::endl;
        }
    }
}
