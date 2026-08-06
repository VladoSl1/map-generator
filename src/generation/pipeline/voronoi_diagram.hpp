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

        virtual void clear();
        bool isPolygonClosed(size_t polygonIndex) const;
        VoronoiDiagram extractVoronoiSubset(size_t seedStartIndex, size_t seedEndIndex) const;
    };

    struct DynamicVoronoiDiagram : public VoronoiDiagram
    {
        std::vector<math::EdgeI> edges;
        std::vector<math::TriangleI> delaunayTriangles;

        void generate(std::vector<math::Point2Dd>& triangleSeeds,
                      const std::vector<math::TriangleI>& triangleIndices);

        /* alg: https://en.wikipedia.org/wiki/Lloyd%27s_algorithm
         * fixedTopology: Difference from the original algorithm is that we are not recalculating the Voronoi diagram from seeds, but rather keeping the same topology and just moving the seeds to the centroids of their polygons.
         * */
        void relax(bool fixedTopology = false);
        void clear() override;

    private:
        void assignEdgeToPolygon(size_t edgeIndex, size_t polygonIndex);
        void convertEdgePolygonsToVertexPolygons();
    };

    /* TODO: Apparently it is possible to apply lloyd's relaxation using a triangulation structure directly */

    static std::vector<math::Point2Dd> calculateVoronoiVertices(const std::vector<math::Point2Dd>& trianglePoints,
                                                    const std::vector<math::TriangleI>& triangleIndices);

}
