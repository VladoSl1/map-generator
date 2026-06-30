#include "voronoi_diagram.hpp"


namespace generation::pipeline
{
    VoronoiDiagram generateVoronoi(std::vector<math::Point2Di> trianglePoints,
                                   std::vector<math::TriangleI> triangleIndices)
    {
        VoronoiDiagram voronoiDiagram;

        voronoiDiagram.vertices = findVoronoiVerticies(trianglePoints, triangleIndices);

        // assuming the triangle points are lexicographically sorted
        for (size_t i = 0; i < triangleIndices.size(); ++i)
        {
            const auto& triangleA = triangleIndices[i];

            for (size_t j = i + 1; j < triangleIndices.size(); ++j)
            {
                const auto& triangleB = triangleIndices[j];

                if (shareEdge(triangleA, triangleB))
                {
                    voronoiDiagram.edges.push_back(math::EdgeI{i, j});

                }
                else
                {
                }
            }
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
