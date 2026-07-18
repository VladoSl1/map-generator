#include "pipeline.hpp"

#include "core/math/point2d.hpp"

#include "generation/pipeline/delaunay_triangulation.hpp"

#include "core/utils/debug.hpp"
#include <vector>


namespace generation::pipeline
{
    VoronoiDiagram generateFromPoints(std::vector<math::Point2Dd> seedPoints)
    {
        auto triangles = pipeline::triangulate(seedPoints);

        VoronoiDiagram voronoiDiagram;

        voronoiDiagram.generate(seedPoints, triangles);

        return voronoiDiagram;
    }
}
