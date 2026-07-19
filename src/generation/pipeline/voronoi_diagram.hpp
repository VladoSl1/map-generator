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
        std::vector<math::EdgeI> edges;
        std::vector<math::PolygonI> polygons;         // PolygonI elements are indices of edges in the edges vector

        std::vector<bool> openPolygons; // true if polygon is open

        void assignEdgeToPolygon(size_t edgeIndex, size_t polygonIndex);

        void generate(std::vector<math::Point2Dd>& trianglePoints,
                      const std::vector<math::TriangleI>& triangleIndices);

        void clear();

        bool isPolygonClosed(size_t polygonIndex) const;

        VoronoiDiagram extractVoronoiSubset(size_t seedStartIndex, size_t seedEndIndex) const;

    };

    std::vector<math::Point2Dd> findVoronoiVertices(const std::vector<math::Point2Dd>& trianglePoints,
                                                    const std::vector<math::TriangleI>& triangleIndices);

    std::vector<math::Point2Dd> relaxVoronoiDiagram(const VoronoiDiagram& voronoiDiagram);
}
