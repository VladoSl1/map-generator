#include "pipeline.hpp"

#include "core/utils/debug.hpp"
#include "generation/pipeline/voronoi_diagram.hpp"
#include "generation/pipeline/delaunay_triangulation.hpp"

#include "core/math/point2d.hpp"

#include <vector>

#include "core/utils/debug.hpp"

namespace generation::pipeline
{
    DynamicVoronoiDiagram generateFromPoints(std::vector<math::Point2Dd> seedPoints)
    {
        // startClock("Generating Voronoi diagram from points");
        auto triangles = pipeline::triangulate(seedPoints);
        // stopClock("Generating Voronoi diagram from points");

        DynamicVoronoiDiagram voronoiDiagram;

        // startClock("Generating Voronoi diagram from triangles");
        voronoiDiagram.generate(std::move(seedPoints), std::move(triangles));
        // stopClock("Generating Voronoi diagram from triangles");

        return voronoiDiagram;
    }
}
