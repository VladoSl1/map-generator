#include "voronoi_diagram.hpp"

#include "core/math/geometry.hpp"
#include "core/math/sorting.hpp"

#include <iostream>

#include "core/utils/debug.hpp"

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
    void VoronoiDiagram::generate(const std::vector<math::Point2Di>& triangleSeeds,
                                  const std::vector<math::TriangleI>& triangleIndices)
    {
        clear();
        polygons.resize(triangleSeeds.size());

        seeds = triangleSeeds; // TODO: consider move semantics

        vertices = findVoronoiVertices(triangleSeeds, triangleIndices);

        /* 0 - idx of triangleSeed from which the edge starts
         * 1 - idx of triangleSeed to which the edge goes
         * 2 - the idx of triangle (= idx of voronoi vertex) to which the edge corresponds
         * */
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
            log("{}", i);
            if (i+1 < triangleEdgesWithId.size() &&
                triangleEdgesWithId[i].edge.indices == triangleEdgesWithId[i+1].edge.indices)
            {
                // add edge between the two voronoi vertices corresponding to their respective triangles
                edges.push_back(
                    math::EdgeI{triangleEdgesWithId[i].triangleId, triangleEdgesWithId[i+1].triangleId}
                );

                assignEdgeToPolygon(edges.size() - 1, triangleEdgesWithId[i].edge[0]);
                assignEdgeToPolygon(edges.size() - 1, triangleEdgesWithId[i].edge[1]);

                // since the initial graph is planar, only two triangles can share an edge
                i += 2;
            }
            // the edge should go to infinity
            else
            {
                // TODO: should we handle this case? maybe we can just ignore it for now
                i += 1;
            }

        }
    }



    /* alg: https://en.wikipedia.org/wiki/Delaunay_triangulation#Relationship_with_the_Voronoi_diagram */
    std::vector<math::Point2Di> findVoronoiVertices(const std::vector<math::Point2Di>& trianglePoints,
                                                     const std::vector<math::TriangleI>& triangleIndices)
    {
        std::vector<math::Point2Di> voronoiVertices;
        voronoiVertices.reserve(triangleIndices.size());

        for (const auto& triangle : triangleIndices)
        {
            auto [a, b, c] = triangle.indices;
            math::Point2Dd circumcenter = math::calculateCircumcenter(trianglePoints[a].cast<double>(),
                                                                      trianglePoints[b].cast<double>(),
                                                                      trianglePoints[c].cast<double>());
            voronoiVertices.push_back(circumcenter.cast<int>());
        }

        return voronoiVertices;
    }

    void relaxVoronoiDiagram(VoronoiDiagram& voronoiDiagram)
    {
        std::vector<math::Point2Di> newSeeds(voronoiDiagram.seeds.size());


        // TODO: ignore polygons that are not closed (i.e. have edges that go to infinity)
        for (size_t i = 0; i < voronoiDiagram.polygons.size(); ++i)
        {
            const auto& polygon = voronoiDiagram.polygons[i];
            math::Point2Dd centroid{0, 0};

            for (const auto& edgeIndex : polygon.indices)
            {
                const auto& edge = voronoiDiagram.edges[edgeIndex];
                const auto& vertex1 = voronoiDiagram.vertices[edge[0]];
                const auto& vertex2 = voronoiDiagram.vertices[edge[1]];

                centroid += vertex1.cast<double>();
                centroid += vertex2.cast<double>();
            }

            centroid /= static_cast<double>(polygon.indices.size() * 2);
            newSeeds[i] = centroid.cast<int>();
        }

    }
}
