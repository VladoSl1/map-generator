#pragma once

#include <algorithm>
#include <array>
#include <vector>

namespace math
{
    template<typename T>
    struct Point2D
    {
        T x;
        T y;

        template<typename U>
        Point2D<U> cast() const
        {
            // apply special rounding and clamping when casting from float/double to int/size_t
            if constexpr (std::is_floating_point_v<T> && std::is_integral_v<U>)
            {
                T rounded_x = std::round(x);
                T rounded_y = std::round(y);

                constexpr T min_limit = static_cast<T>(std::numeric_limits<U>::min());
                constexpr T max_limit = static_cast<T>(std::numeric_limits<U>::max());

                rounded_x = std::clamp(rounded_x, min_limit, max_limit);
                rounded_y = std::clamp(rounded_y, min_limit, max_limit);

                return { static_cast<U>(rounded_x), static_cast<U>(rounded_y) };
            }
            else
            {
                // default fallback for safe casts (e.g., int -> float, float -> double)
                return { static_cast<U>(x), static_cast<U>(y) };
            }
        }
    };


    using Point2Di = Point2D<int>;
    using Point2Df = Point2D<float>;
    using Point2Dd = Point2D<double>;


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

    using PolygonI = IndexContainer<std::vector<size_t>>;


    /* algorithm: https://en.wikipedia.org/wiki/Circumcircle#Circumcenter_coordinates */
    Point2Dd calculateCircumcenter(const Point2Dd& A, const Point2Dd& B, const Point2Dd& C);

    bool shareEdge(const TriangleI& triangleA, const TriangleI& triangleB);



    template<typename T, typename Proj>
    void countingSort(std::vector<T>& elements, Proj proj)
    {
        // TODO: consider maybe for better performance adding buffering to avoid multiple allocations
        // TODO: is this function maybe to abstract?

        const int n = elements.size();
        if (n <= 1) return;

        std::vector<size_t> counts(n, 0);

        // count how many times each mapped value appears
        for (const auto& element : elements)
        {
            counts[proj(element)]++;
        }

        // convert counts to prefix sums to determine the final index positions
        for (size_t i = 1; i < n; ++i)
        {
            counts[i] += counts[i - 1];
        }

        // build the sorted output array
        std::vector<T> output(n);
        for (int i = static_cast<int>(n) - 1; i >= 0; --i)
        {
            size_t val = proj(elements[i]);
            output[counts[val] - 1] = std::move(elements[i]); // Optimization: move instead of copy
            counts[val]--;
        }

        elements = std::move(output);
    }

    template<size_t N>
    void countingSortPrimitives(std::vector<IndexPrimitive<N>>& primitives, size_t vertexIndex)
    {
        countingSort(primitives, [vertexIndex](const IndexPrimitive<N>& p) { return p[vertexIndex]; });
    }

    template<size_t N>
    void sortPrimitives(std::vector<IndexPrimitive<N>>& primitives)
    {
        for (int vertexIndex = N-1; vertexIndex >= 0; --vertexIndex)
        {
            countingSortPrimitives(primitives, vertexIndex);
        }
    }


    template<size_t N>
    std::array<EdgeI, N> convertToEdges(const IndexPrimitive<N>& primitive)
    {
        std::array<EdgeI, N> edges;

        for (size_t i = 0; i < N; ++i)
        {
            size_t v1 = primitive[i];
            size_t v2 = primitive[i + 1 == N ? 0 : i + 1];

            edges[i] = EdgeI{{ std::min(v1, v2), std::max(v1, v2) }};
        }

        return edges;
    }
}
