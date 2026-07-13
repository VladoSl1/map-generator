#include "voronoi_diagram.hpp"
#include "core/math/math.hpp"

#include <iostream>

#include "core/utils/debug.hpp"

namespace generation::pipeline
{

    void VoronoiDiagram::assignEdgeToPolygon(size_t edgeIndex, size_t polygonIndex)
    {
        polygons[polygonIndex].indices.push_back(edgeIndex);
    }


    struct EdgeWithOwner
    {
        math::EdgeI edge;
        size_t triangleId;
    };


    /**
     * First, calculate Voronoi vertices by calculating the circumcenters of each triangle.
     * In Voronoi diagram, there is edge between two vertices, iff in Delaunay triangulation
     * the two triangles coresponding to these verticies share an edge. Therefore, it makes
     * sense to process the graph edge-wise. For efficiency, we use radix sort to process the
     * edge in O(n) time.
     * It is possible to directly calculate the Voronoi edges from points without Deulaunay
     * triangles, but this approach is much more complicated.
     * */
    VoronoiDiagram generateVoronoi(const std::vector<math::Point2Di>& triangleSeeds,
                                   const std::vector<math::TriangleI>& triangleIndices)
    {
        VoronoiDiagram voronoiDiagram;
        voronoiDiagram.polygons.resize(triangleSeeds.size());

        voronoiDiagram.vertices = findVoronoiVerticies(triangleSeeds, triangleIndices);

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

            for (int j = 0; j < triEdges.size(); ++j)
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
                voronoiDiagram.edges.push_back(
                    math::EdgeI{triangleEdgesWithId[i].triangleId, triangleEdgesWithId[i+1].triangleId}
                );

                voronoiDiagram.assignEdgeToPolygon(voronoiDiagram.edges.size() - 1, triangleEdgesWithId[i].edge[0]);
                voronoiDiagram.assignEdgeToPolygon(voronoiDiagram.edges.size() - 1, triangleEdgesWithId[i].edge[1]);

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

        return voronoiDiagram;
    }



    /* alg: https://en.wikipedia.org/wiki/Delaunay_triangulation#Relationship_with_the_Voronoi_diagram */
    std::vector<math::Point2Di> findVoronoiVerticies(const std::vector<math::Point2Di>& trianglePoints,
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


}
