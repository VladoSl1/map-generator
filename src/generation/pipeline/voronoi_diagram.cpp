#include "voronoi_diagram.hpp"
#include "core/config.hpp"
#include "core/math/geometry.hpp"
#include "core/math/point2d.hpp"
#include "core/math/sorting.hpp"
#include "core/utils/debug.hpp"

#include <cassert>

namespace generation::pipeline
{
    void VoronoiDiagram::assignEdgeToPolygon(size_t edgeIndex, size_t polygonIndex)
    {
        polygons[polygonIndex].indices.push_back(edgeIndex);
    }

    void VoronoiDiagram::clear()
    {
        seeds.clear();
        vertices.clear();
        edges.clear();
        polygons.clear();
        openPolygons.clear();
    }

    struct EdgeWithOwner
    {
        math::EdgeI edge;
        size_t triangleId;
    };

    /**
     * First, calculate Voronoi vertices by calculating the circumcenters of each triangle.
     * In Voronoi diagram, there is edge between two vertices, iff in Delaunay triangulation
     * the two triangles corresponding to these verticies share an edge. Therefore, it makes
     * sense to process the graph edge-wise. For efficiency, we use radix sort to process the
     * edge in O(n) time.
     * It is possible to directly calculate the Voronoi edges from points without Delaunay
     * triangles, but this approach is much more complicated.
     * */
    void VoronoiDiagram::generate(std::vector<math::Point2Dd>& triangleSeeds,
                                  const std::vector<math::TriangleI>& triangleIndices)
    {
        clear();
        polygons.resize(triangleSeeds.size());
        openPolygons.assign(triangleSeeds.size(), false);

        seeds = triangleSeeds; //TODO: consider move semantics
        vertices = findVoronoiVertices(seeds, triangleIndices);

        std::vector<EdgeWithOwner> triangleEdgesWithId;
        triangleEdgesWithId.reserve(triangleIndices.size() * 3);

        for (size_t i = 0; i < triangleIndices.size(); ++i)
        {
            // array of edges of the triangle, each edge is represented
            // by two indices of triangleSeeds
            auto triEdges = math::convertToEdges(triangleIndices[i]);
            for (size_t j = 0; j < triEdges.size(); ++j)
            {
                triangleEdgesWithId.push_back({ triEdges[j], i });
            }
        }

        math::countingSort(triangleEdgesWithId, [](const EdgeWithOwner& e) { return e.edge[1]; });
        math::countingSort(triangleEdgesWithId, [](const EdgeWithOwner& e) { return e.edge[0]; });

        size_t i = 0;
        while (i < triangleEdgesWithId.size())
        {
            if (i+1 < triangleEdgesWithId.size() &&
                triangleEdgesWithId[i].edge.indices == triangleEdgesWithId[i+1].edge.indices)
            {
                // edge shared by 2 Delaunay triangles (Internal Polygon Edge)
                edges.push_back(
                    math::EdgeI{triangleEdgesWithId[i].triangleId, triangleEdgesWithId[i+1].triangleId}
                );

                assignEdgeToPolygon(edges.size() - 1, triangleEdgesWithId[i].edge[0]);
                assignEdgeToPolygon(edges.size() - 1, triangleEdgesWithId[i].edge[1]);

                i += 2;
            }
            else // the edge should go to ininite polygon (External Polygon Edge)
            {
                openPolygons[triangleEdgesWithId[i].edge[0]] = true;
                openPolygons[triangleEdgesWithId[i].edge[1]] = true;

                i += 1;
            }
        }
    }

    bool VoronoiDiagram::isPolygonClosed(size_t polygonIndex) const
    {
        if (polygonIndex >= openPolygons.size()) return false;
        return !openPolygons[polygonIndex];
    }

    /* alg: https://en.wikipedia.org/wiki/Delaunay_triangulation#Relationship_with_the_Voronoi_diagram */
    std::vector<math::Point2Dd> findVoronoiVertices(const std::vector<math::Point2Dd>& trianglePoints,
                                                     const std::vector<math::TriangleI>& triangleIndices)
    {
        std::vector<math::Point2Dd> voronoiVertices;
        voronoiVertices.reserve(triangleIndices.size());

        for (const auto& triangle : triangleIndices)
        {
            auto [a, b, c] = triangle.indices;
            auto circumcenter = math::calculateCircumcenter(trianglePoints[a], trianglePoints[b], trianglePoints[c]);
            if (!circumcenter.has_value()) continue;  // skip degenerate triangles
            voronoiVertices.push_back(circumcenter.value());
        }

        return voronoiVertices;
    }

    std::vector<math::Point2Dd> relaxVoronoiDiagram(const VoronoiDiagram& voronoiDiagram)
    {
        std::vector<math::Point2Dd> newSeeds(voronoiDiagram.seeds.size());

        for (size_t i = 0; i < voronoiDiagram.polygons.size(); ++i)
        {
            if (!voronoiDiagram.isPolygonClosed(i)) // ignore polygons whose edges go to infinity
            {
                newSeeds[i] = voronoiDiagram.seeds[i];
                continue;
            }

            math::Point2Dd centroid{0, 0};

            for (const auto& edgeIndex : voronoiDiagram.polygons[i].indices)
            {
                const auto& edge = voronoiDiagram.edges[edgeIndex];
                centroid += voronoiDiagram.vertices[edge[0]].cast<double>();
                centroid += voronoiDiagram.vertices[edge[1]].cast<double>();
            }

            centroid /= static_cast<double>(voronoiDiagram.polygons[i].indices.size() * 2);
            newSeeds[i] = centroid;
        }

        return newSeeds;
    }
}
