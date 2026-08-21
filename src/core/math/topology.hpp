#pragma once

#include "static_vector.hpp"

#include <algorithm>
#include <array>
#include <vector>


namespace math
{
    template <typename Container>
    struct IndexContainer
    {
        /* kept public so that it is aggregate initializable,
         * there are no invariants to maintain,
         * making it private would require a lot of boilerplate.
         * */
        Container indices;

        constexpr void sort_indices()
        {
            std::ranges::sort(indices);
        }

        constexpr auto size() const { return indices.size(); }
        constexpr decltype(auto) operator[](size_t index) const { return indices[index]; }
        constexpr decltype(auto) operator[](size_t index)       { return indices[index]; }
    };

    template <size_t N>
    using IndexPrimitive = IndexContainer<std::array<size_t, N>>;

    using EdgeI     = IndexPrimitive<2>;
    using TriangleI = IndexPrimitive<3>;

    inline constexpr size_t MAX_POLYGON_DEGREE = 32;
    static_assert(MAX_POLYGON_DEGREE <= 64, "bitmask is 64 bits wide");
    using PolygonIContainer = StaticVector<size_t, MAX_POLYGON_DEGREE>;
    using PolygonI = IndexContainer<PolygonIContainer>;

    bool doesShareEdge(const TriangleI& triangleA, const TriangleI& triangleB);

    PolygonI getIndicesPolygon(const std::vector<EdgeI>& edges,
                               const PolygonI& edgePolygon);

    /* Converts e.g. triangle indices to edges. Edge elements are sorted. We are assuming
     * that the IndexPrimitive creates cycle from vertices */
    template<size_t N>
    std::array<EdgeI, N> convertToEdges(const IndexPrimitive<N>& primitive)
    {
        std::array<EdgeI, N> edges;

        for (size_t i = 0; i < N; ++i)
        {
            const size_t v1 = primitive[i];
            const size_t v2 = primitive[i + 1 == N ? 0 : i + 1];

            edges[i] = EdgeI{{ std::min(v1, v2), std::max(v1, v2) }};
        }

        return edges;
    }
}
