#include "pipeline.hpp"

#include "generation/pipeline/voronoi_diagram.hpp"
#include "generation/pipeline/delaunay_triangulation.hpp"

#include "core/math/point2d.hpp"

#include <cstdint>
#include <vector>


namespace generation::pipeline
{
    DynamicVoronoiDiagram generateFromPoints(std::vector<math::Point2Dd> seedPoints)
    {
        auto triangles = pipeline::triangulate(seedPoints);

        DynamicVoronoiDiagram voronoiDiagram;

        voronoiDiagram.generate(seedPoints, triangles);

        return voronoiDiagram;
    }
}
