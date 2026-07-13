#include "pipeline.hpp"

#include "core/math/math.hpp"
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

        // add boundary points for debugging purposes
        for (int i = 0; i < m_width; i += 100)
        {
            points.emplace_back(math::Point2Di{i, 0});
            points.emplace_back(math::Point2Di{i, m_height});
        }
        for (int i = 0; i < m_height; i += 100)
        {
            points.emplace_back(math::Point2Di{0, i});
            points.emplace_back(math::Point2Di{m_width, i});
        }

        triangles = pipeline::triangulate(points);

        // math::sortPrimitives(triangles);

        voronoiDiagram.generate(points, triangles);

        voronoiVertices = voronoiDiagram.vertices;

        // std::cout << voronoiDiagram.vertices.size() << " voronoi vertices generated" << std::endl;
        // for (const auto& edge : voronoiDiagram.edges)
        // {
        //     std::cout << edge[0] << " -> " << edge[1] << std::endl;
        // }
        //
        // std::cout << std::endl;
        //
        // for (int i = 0; i < triangles.size(); ++i)
        // {
        //     std::cout << i << ": " << triangles[i][0] << " " << triangles[i][1] << " " << triangles[i][2] << std::endl;
        //
        // }
    }
}
