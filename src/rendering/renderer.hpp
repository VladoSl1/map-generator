#include <raylib.h>

#include "generation/chunkmanager.hpp"

#include "core/math/point2d.hpp"
#include "core/math/topology.hpp"

#include <vector>


namespace renderer
{
    void renderPoints(const std::vector<math::Point2Dd>& points, Color color);

    void renderTriangles(const std::vector<math::Point2Dd>& points,
                         const std::vector<math::TriangleI>& triangles,
                         Color color = GREEN);

    void renderEdges(const std::vector<math::Point2Dd>& points,
                     const std::vector<math::EdgeI>& edges,
                     Color color);

    /* Expects polygons to be clockwise ordered */
    void renderPolygons(const generation::pipeline::VoronoiDiagram& diagram,
                        Color color);

    void renderChunkGrid(const generation::ChunkManager& chunkManager);
}
