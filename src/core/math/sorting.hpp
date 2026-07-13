#pragma once

#include "topology.hpp"

namespace math
{
    template<typename T, typename Proj>
    void countingSort(std::vector<T>& elements, Proj proj)
    {
        // TODO: consider maybe for better performance adding buffering to avoid multiple allocations
        // TODO: is this function maybe to abstract?
        // TODO: the sort assumes that the projected values are in range [0, elemts.size())]

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
            output[counts[val] - 1] = std::move(elements[i]);
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
}
