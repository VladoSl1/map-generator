#include "voronoi_diagram.hpp"
#include "core/math/math.hpp"


namespace generation::pipeline
{
    VoronoiDiagram generateVoronoi(std::vector<math::Point2Di> trianglePoints,
                                   std::vector<math::TriangleI> triangleIndices)
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

        for (size_t i = 0; i < edgesWithId.size(); ++i)
        {



        }


        return voronoiDiagram;
    }



    /* alg: https://en.wikipedia.org/wiki/Delaunay_triangulation#Relationship_with_the_Voronoi_diagram */
    std::vector<math::Point2Di> findVoronoiVerticies(std::vector<math::Point2Di> trianglePoints,
                                                     std::vector<math::TriangleI> triangleIndices)
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
