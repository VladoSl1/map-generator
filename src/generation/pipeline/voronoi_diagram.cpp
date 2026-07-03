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
        std::vector<math::TriangleI> triangleEdgesWithId;
        triangleEdgesWithId.reserve(triangleIndices.size() * 3);

        for (size_t i = 0; i < triangleIndices.size(); ++i)
        {
            // array of edges of the triangle, each edge is represented
            // by two indices of triangleSeeds
            auto triEdges = math::convertToEdges(triangleIndices[i]);

            for (int j = 0; j < triEdges.size(); ++j)
            {
                triangleEdgesWithId.emplace_back(math::TriangleI{{ triEdges[j][0], triEdges[j][1], i }});
            }
        }


        math::bucketSortPrimitives(triangleEdgesWithId, 1);
        math::bucketSortPrimitives(triangleEdgesWithId, 0);

        log("triangleEdgesWithId.size() = {}", triangleEdgesWithId.size());
        log("voronoiDiagram.vertices.size() = {}", voronoiDiagram.vertices.size());


        size_t i = 0;
        while (i < triangleEdgesWithId.size())
        {
            log("{}", i);
            if (i+1 < triangleEdgesWithId.size() &&
                triangleEdgesWithId[i][0] == triangleEdgesWithId[i+1][0] &&
                triangleEdgesWithId[i][1] == triangleEdgesWithId[i+1][1])
            {
                // add edge between the two voronoi vertices corresponding to their respective triangles
                voronoiDiagram.edges.emplace_back(
                    math::EdgeI{triangleEdgesWithId[i][2], triangleEdgesWithId[i+1][2]}
                );

                voronoiDiagram.assignEdgeToPolygon(voronoiDiagram.edges.size() - 1, triangleEdgesWithId[i][0]);
                voronoiDiagram.assignEdgeToPolygon(voronoiDiagram.edges.size() - 1, triangleEdgesWithId[i][1]);

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
