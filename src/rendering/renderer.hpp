#include <raylib.h>

#include "core/math/point2d.hpp"
#include "core/math/topology.hpp"

#include <vector>

#include "generation/chunkmanager.hpp"


namespace renderer
{
    void renderPoints(const std::vector<math::Point2Dd>& points, Color color);
    void renderTriangles(const std::vector<math::Point2Dd>& points,
                         const std::vector<math::TriangleI>& triangles,
                         Color color = GREEN);
    void renderEdges(const std::vector<math::Point2Dd>& points,
                     const std::vector<math::EdgeI>& edges,
                     Color color);

    void renderChunkGrid(const generation::ChunkManager& chunkManager);
}
