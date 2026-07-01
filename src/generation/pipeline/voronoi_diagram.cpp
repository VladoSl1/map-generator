#include "voronoi_diagram.hpp"
#include "core/math/math.hpp"


namespace generation::pipeline
{

    /**
     * First, calculate Voronoi vertices by calculating the circumcenters of each triangle.
     * In Voronoi diagram, there is edge between two vertices, iff in Delaunay triangulation
     * the two triangles coresponding to these verticies share an edge. Therefore, it makes
     * sense to process the graph edge-wise. For efficiency, we use radix sort to process the
     * edge in O(n) time.
     * */
    VoronoiDiagram generateVoronoi(const std::vector<math::Point2Di>& trianglePoints,
                                   const std::vector<math::TriangleI>& triangleIndices)
    {
        VoronoiDiagram voronoiDiagram;

        voronoiDiagram.vertices = findVoronoiVerticies(trianglePoints, triangleIndices);

        std::vector<math::TriangleI> edgesWithId;
        edgesWithId.reserve(triangleIndices.size() * 3);

        for (size_t i = 0; i < triangleIndices.size(); ++i)
        {
            auto triEdges = math::convertToEdges(triangleIndices[i]);

            for (int j = 0; j < triEdges.size(); ++j)
            {
                edgesWithId.emplace_back(math::TriangleI{{ triEdges[j][0], triEdges[j][1], i }});
            }
        }

        math::bucketSortPrimitives(edgesWithId, 1);
        math::bucketSortPrimitives(edgesWithId, 0);


        size_t i = 0;
        while (i < edgesWithId.size())
        {
            // since the initial graph is planar, only two triangles can share an edge
            if (edgesWithId[i][0] == edgesWithId[i+1][0] && edgesWithId[i][1] == edgesWithId[i+1][1])
            {
                voronoiDiagram.edges.emplace_back(math::EdgeI{edgesWithId[i][2], edgesWithId[i+1][2]});
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
