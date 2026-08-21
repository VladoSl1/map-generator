#include "voronoi_diagram.hpp"

#include "core/math/geometry.hpp"
#include "core/math/point2d.hpp"
#include "core/math/sorting.hpp"
#include "core/math/constants.hpp"

#include "core/math/topology.hpp"
#include "delaunay_triangulation.hpp"

#include "core/utils/debug.hpp"

#include <cassert>

namespace generation::pipeline
{

    void VoronoiDiagram::clear()
    {
        seeds.clear();
        vertices.clear();
        polygons.clear();
        openPolygons.clear();
    }

    void DynamicVoronoiDiagram::clear()
    {
        VoronoiDiagram::clear();
        edges.clear();
        delaunayTriangles.clear();
    }

    void DynamicVoronoiDiagram::assignEdgeToPolygon(size_t edgeIndex, size_t polygonIndex)
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
    void DynamicVoronoiDiagram::generate(std::vector<math::Point2Dd> triangleSeeds,
                                         std::vector<math::TriangleI> triangleIndices)
    {
        clear();
        polygons.resize(triangleSeeds.size());
        openPolygons.assign(triangleSeeds.size(), false);

        seeds = std::move(triangleSeeds);
        delaunayTriangles = std::move(triangleIndices);
        vertices = calculateVoronoiVertices(seeds, delaunayTriangles);

        std::vector<EdgeWithOwner> triangleEdgesWithId;
        triangleEdgesWithId.reserve(delaunayTriangles.size() * 3);

        for (size_t i = 0; i < delaunayTriangles.size(); ++i)
        {
            // array of edges of the triangle, each edge is represented
            // by two indices of triangleSeeds
            auto triEdges = math::convertToEdges(delaunayTriangles[i]);
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
                // edge shared by 2 delaunay triangles
                edges.push_back(
                    math::EdgeI{triangleEdgesWithId[i].triangleId, triangleEdgesWithId[i+1].triangleId}
                );

                assignEdgeToPolygon(edges.size() - 1, triangleEdgesWithId[i].edge[0]);
                assignEdgeToPolygon(edges.size() - 1, triangleEdgesWithId[i].edge[1]);

                i += 2;
            }
            else // the edge should go to ininite polygon
            {
                openPolygons[triangleEdgesWithId[i].edge[0]] = true;
                openPolygons[triangleEdgesWithId[i].edge[1]] = true;

                i += 1;
            }
        }

        convertEdgePolygonsToVertexPolygons();
    }

    /*
     *
     *
     */
    void DynamicVoronoiDiagram::convertEdgePolygonsToVertexPolygons()
    {
        for (size_t p = 0; p < polygons.size(); ++p)
        {
            if (openPolygons[p])
            {
                // the edges are not ordered
                // there are rouhgly ~30 open polygons out of 27000 polygons, so this approach should be fast enough
                math::PolygonIContainer vertIndices;
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

                double signedArea = math::calculateSignedPolygonArea(vertices, polygons[p]);

                // If the signed area is negative, the polygon is Clockwise.
                // Reverse the indices to make it Counter-Clockwise.
                if (signedArea > 0.0)
                {
                    std::ranges::reverse(polygons[p].indices.begin(), polygons[p].indices.end());
                }
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

    /* This function is exctracts subset of Voronoi diagram corresponding to a subset of seeds.
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
                // if the vertex hasn't been added to the subset yet, map and push it
                if (vertexMap[oldVertIdx] == UNMAPPED)
                {
                    vertexMap[oldVertIdx] = subsetVoronoi.vertices.size();
                    subsetVoronoi.vertices.push_back(vertices[oldVertIdx]);
                }

                // add the newly mapped vertex index to the local polygon
                subsetVoronoi.polygons[localIndex].indices.push_back(vertexMap[oldVertIdx]);
            }
        }

        return subsetVoronoi;
    }

    void DynamicVoronoiDiagram::relax(bool fixedTopology)
    {
        std::vector<math::Point2Dd> newSeeds = seeds;

        for (size_t i = 0; i < polygons.size(); ++i)
        {
            if (!isPolygonClosed(i) || polygons[i].indices.empty())
            {
                continue;
            }

            newSeeds[i] = math::calculatePolygonCentroid(vertices, polygons[i]);
        }

        if (fixedTopology)
        {
            seeds = std::move(newSeeds);
            vertices = calculateVoronoiVertices(seeds, delaunayTriangles);
        }
        else
        {
            auto newTriangles = triangulate(newSeeds);
            generate(newSeeds, newTriangles);
        }
    }



    /* alg: https://en.wikipedia.org/wiki/Delaunay_triangulation#Relationship_with_the_Voronoi_diagram */
    std::vector<math::Point2Dd> calculateVoronoiVertices(const std::vector<math::Point2Dd>& trianglePoints,
                                                     const std::vector<math::TriangleI>& triangleIndices)
    {
        assert(!trianglePoints.empty() && "Triangle points cannot be empty.");
        assert(!triangleIndices.empty() && "Triangle indices cannot be empty.");

        std::vector<math::Point2Dd> voronoiVertices;
        voronoiVertices.reserve(triangleIndices.size());

        for (const auto& triangle : triangleIndices)
        {
            const auto [a, b, c] = triangle.indices;
            const auto circumcenter = math::calculateCircumcenter(trianglePoints[a], trianglePoints[b], trianglePoints[c]);
            if (circumcenter.has_value())
            {
                voronoiVertices.push_back(circumcenter.value());
            }
            else  // triangle is degenerate, circumcenter is undefined
            {
                // fallback to the centroid of the triangle
                voronoiVertices.push_back((trianglePoints[a] + trianglePoints[b] + trianglePoints[c]) / 3.0);
            }
        }

        return voronoiVertices;
    }
}
