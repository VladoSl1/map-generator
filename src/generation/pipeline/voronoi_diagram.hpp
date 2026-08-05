#pragma once

#include <vector>

#include "core/math/point2d.hpp"
#include "core/math/topology.hpp"

namespace generation::pipeline
{

    // TODO: add STATIC and DYNAMIC voronoi diagram types
    // DYNAMIC: can be relaxed efficiently - we would not need to run Delaunay triangulation
    // STATIC: data relevant only for relaxation would be deleted

    struct VoronoiDiagram
    {
        std::vector<math::Point2Dd> seeds;
        std::vector<math::Point2Dd> vertices;
        std::vector<math::EdgeI> edges; // TODO: remove edges, they are redundant
        std::vector<math::PolygonI> polygons;  // polygon elements are indices of vertices

        std::vector<bool> openPolygons; // true if polygon is open

        void assignEdgeToPolygon(size_t edgeIndex, size_t polygonIndex);

        void generate(std::vector<math::Point2Dd>& triangleSeeds,
                      const std::vector<math::TriangleI>& triangleIndices);

        void clear();

        bool isPolygonClosed(size_t polygonIndex) const;

        VoronoiDiagram extractVoronoiSubset(size_t seedStartIndex, size_t seedEndIndex) const;

    private:
        /* The order of vertices is CLOCKWISE -> this is very important for rendering */
        void convertEdgePolygonsToVertexPolygons();
    };

    std::vector<math::Point2Dd> calculateVoronoiVertices(const std::vector<math::Point2Dd>& trianglePoints,
                                                    const std::vector<math::TriangleI>& triangleIndices);

    std::vector<math::Point2Dd> relaxVoronoiDiagram(const VoronoiDiagram& voronoiDiagram);
}
