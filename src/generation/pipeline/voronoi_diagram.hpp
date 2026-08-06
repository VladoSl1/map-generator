#pragma once

#include <vector>

#include "core/math/point2d.hpp"
#include "core/math/topology.hpp"

namespace generation::pipeline
{
    struct VoronoiDiagram
    {
        std::vector<math::Point2Dd> seeds;
        std::vector<math::Point2Dd> vertices;
        std::vector<math::PolygonI> polygons;  // Polygon elements are indices of vertices
        std::vector<bool> openPolygons;        // True if polygon is open to infinity

        void clear();
        bool isPolygonClosed(size_t polygonIndex) const;
        VoronoiDiagram extractVoronoiSubset(size_t seedStartIndex, size_t seedEndIndex) const;
    };

    struct DynamicVoronoiDiagram : public VoronoiDiagram
    {
        std::vector<math::EdgeI> edges;
        std::vector<math::TriangleI> delaunayTriangles;

        void generate(std::vector<math::Point2Dd>& triangleSeeds,
                      const std::vector<math::TriangleI>& triangleIndices);

        /* The order of points stays the same, meaning structure from original voronoi diagram can be reused
         * Fixed-topology Lloyd's relaxation iteration
         * */
        void relax();

    private:
        void assignEdgeToPolygon(size_t edgeIndex, size_t polygonIndex);
        void convertEdgePolygonsToVertexPolygons();
    };

    /* TODO: Apparently it is possible to apply lloyd's relaxation using a triangulation structure directly */

    static std::vector<math::Point2Dd> calculateVoronoiVertices(const std::vector<math::Point2Dd>& trianglePoints,
                                                    const std::vector<math::TriangleI>& triangleIndices);

}
