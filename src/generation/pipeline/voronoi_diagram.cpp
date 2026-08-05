#include "voronoi_diagram.hpp"

#include "core/math/geometry.hpp"
#include "core/math/point2d.hpp"
#include "core/math/sorting.hpp"

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
     * edges in O(n) time.
     * We are constructing the Voronoi diagram from finite set of points, therefore some of the
     * Voronoi polygons will be open (external). We ignore external edges and keep track which
     * polygons are open. This is useful for relaxation.
     *
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
        vertices = calculateVoronoiVertices(seeds, triangleIndices);

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

        convertEdgePolygonsToVertexPolygons();
    }

    void VoronoiDiagram::convertEdgePolygonsToVertexPolygons()
    {
        for (size_t p = 0; p < polygons.size(); ++p)
        {
            if (openPolygons[p])
            {
                // TODO: right now we are just collecting all vertices of the edges, but they are not ordered
                std::vector<size_t> vertIndices;
                for (size_t edgeIdx : polygons[p].indices)
                {
                    const auto& edge = edges[edgeIdx];
                    if (std::ranges::find(vertIndices.begin(), vertIndices.end(), edge[0]) == vertIndices.end())
                    {
                        vertIndices.push_back(edge[0]);
                    }
                    if (std::ranges::find(vertIndices.begin(), vertIndices.end(), edge[1]) == vertIndices.end())
                    {
                        vertIndices.push_back(edge[1]);
                    }
                }
                polygons[p].indices = std::move(vertIndices);
            }
            else
            {
                polygons[p] = math::getIndicesPolygon(edges, polygons[p]);
                math::orderPolygonClockwise(vertices, polygons[p]);
            }
        }
    }

    bool VoronoiDiagram::isPolygonClosed(size_t polygonIndex) const
    {
        if (polygonIndex >= openPolygons.size())
        {
            return false;
        }
        return !openPolygons[polygonIndex];
    }

    /* Purpose of this function is to exctract subset of Voronoi diagram corresponding to a subset of seeds.
     * To every seed corresponds a polygon, and to every polygon corresponds a set of vertices.
     * This function maps the vertices of the original Voronoi diagram to the new subset Voronoi diagram,
     * and updates the edges and polygons accordingly.
     */
    VoronoiDiagram VoronoiDiagram::extractVoronoiSubset(size_t seedStartIndex, size_t seedEndIndex) const
    {
        VoronoiDiagram subsetVoronoi;
        const size_t numSeeds = seedEndIndex - seedStartIndex;

        subsetVoronoi.seeds = std::vector<math::Point2Dd>(seeds.begin() + seedStartIndex,
                                                          seeds.begin() + seedEndIndex);

        subsetVoronoi.vertices.reserve(vertices.size()); // there may be vertices corresponding to other seeds, so this is just lower bound

        subsetVoronoi.polygons.resize(numSeeds);
        subsetVoronoi.openPolygons.resize(numSeeds);

        constexpr size_t UNMAPPED = std::numeric_limits<size_t>::max();

        std::vector<size_t> vertexMap(vertices.size(), UNMAPPED);

        for (size_t i = seedStartIndex; i < seedEndIndex; ++i)
        {
            size_t localIndex = i - seedStartIndex;
            // subsetVoronoi.openPolygons[localIndex] = false;
            subsetVoronoi.openPolygons[localIndex] = openPolygons[i];

            for (size_t oldVertIdx : polygons[i].indices)
            {
                // If the vertex hasn't been added to the subset yet, map and push it
                if (vertexMap[oldVertIdx] == UNMAPPED)
                {
                    vertexMap[oldVertIdx] = subsetVoronoi.vertices.size();
                    subsetVoronoi.vertices.push_back(vertices[oldVertIdx]);
                }

                // Add the newly mapped vertex index to the local polygon
                subsetVoronoi.polygons[localIndex].indices.push_back(vertexMap[oldVertIdx]);
            }
        }

        for (const auto& edge : edges)
        {
            size_t newV1 = vertexMap[edge[0]];
            size_t newV2 = vertexMap[edge[1]];

            // retain only edges where both vertices exist in this subset
            if (newV1 != UNMAPPED && newV2 != UNMAPPED)
            {
                subsetVoronoi.edges.push_back(math::EdgeI{{newV1, newV2}});
            }
        }

        return std::move(subsetVoronoi);
    }


    /* alg: https://en.wikipedia.org/wiki/Delaunay_triangulation#Relationship_with_the_Voronoi_diagram */
    std::vector<math::Point2Dd> calculateVoronoiVertices(const std::vector<math::Point2Dd>& trianglePoints,
                                                     const std::vector<math::TriangleI>& triangleIndices)
    {
        std::vector<math::Point2Dd> voronoiVertices;
        voronoiVertices.reserve(triangleIndices.size());

        for (const auto& triangle : triangleIndices)
        {
            auto [a, b, c] = triangle.indices;
            auto circumcenter = math::calculateCircumcenter(trianglePoints[a], trianglePoints[b], trianglePoints[c]);
            if (!circumcenter.has_value())   // skip degenerate triangles
            {
                continue;
            }
            voronoiVertices.push_back(circumcenter.value());
        }

        return voronoiVertices;
    }

    /* alg: https://en.wikipedia.org/wiki/Lloyd%27s_algorithm */
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

            for (const auto& vertexIndex : voronoiDiagram.polygons[i].indices)
            {
                centroid += voronoiDiagram.vertices[vertexIndex].cast<double>();
            }

            centroid /= static_cast<double>(voronoiDiagram.polygons[i].indices.size());
            newSeeds[i] = centroid;
        }

        return newSeeds;
    }
}
