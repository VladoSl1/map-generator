#include <gtest/gtest.h>

#include <chrono>
#include <utility>
#include <iomanip>

#include "generation/pipeline/pipeline.hpp"
#include "core/utils/random.hpp"
#include "generation/pipeline/delaunay_triangulation.hpp"


std::vector<math::TriangleI> generateTriangleData(math::RngEngine& rng, size_t count) {
    std::vector<math::TriangleI> triangles;

    math::UnifIntDistribution dist(0, count-1);

    for (size_t i = 0; i < count; ++i) {
        math::TriangleI triangle;
        triangle.a = math::getRandomInt(rng, dist);
        triangle.b = math::getRandomInt(rng, dist);
        triangle.c = math::getRandomInt(rng, dist);

        triangle.sort_indices();
        triangles.push_back(triangle);
    }

    return triangles;
}

void standardSortTriangles(std::vector<math::TriangleI>& triangles) {
    std::sort(triangles.begin(), triangles.end(), [](const math::TriangleI& a, const math::TriangleI& b) {
        if (a[0] != b[0]) return a[0] < b[0];
        if (a[1] != b[1]) return a[1] < b[1];
        return a[2] < b[2];
    });
}

class TriangleSortPerfTest : public ::testing::TestWithParam<size_t> {
protected:
    // Helper to verify both sorting methods yielded identical results
    void AssertVectorsEqual(const std::vector<math::TriangleI>& a, const std::vector<math::TriangleI>& b) {
        ASSERT_EQ(a.size(), b.size());
        for (size_t i = 0; i < a.size(); ++i) {
            ASSERT_EQ(a[i][0], b[i][0]);
            ASSERT_EQ(a[i][1], b[i][1]);
            ASSERT_EQ(a[i][2], b[i][2]);
        }
    }
};

// --- The Benchmark Test Case ---
TEST_P(TriangleSortPerfTest, CompareExecutionTime) {
    size_t dataSize = GetParam();

    math::RngEngine rng(42); // Fixed seed for reproducibility

    auto datasetCustom = generateTriangleData(rng, dataSize);
    auto datasetStd = datasetCustom;

    // 1. Benchmark Custom Bucket/Radix Sort
    auto start = std::chrono::high_resolution_clock::now();
    generation::pipeline::sortTriangles(datasetCustom);
    auto end = std::chrono::high_resolution_clock::now();
    double customMs = std::chrono::duration<double, std::milli>(end - start).count();

    // 2. Benchmark std::sort
    start = std::chrono::high_resolution_clock::now();
    standardSortTriangles(datasetStd);
    end = std::chrono::high_resolution_clock::now();
    double stdMs = std::chrono::duration<double, std::milli>(end - start).count();

    // 3. Export data to GTest XML/JSON report properties
    RecordProperty("ElementCount", std::to_string(dataSize));
    RecordProperty("CustomSort_ms", std::to_string(customMs));
    RecordProperty("StdSort_ms", std::to_string(stdMs));

    std::cout << "[  PERF    ] Size: " << std::setw(7) << dataSize
              << " | Custom Sort: " << std::setw(8) << std::fixed << std::setprecision(3) << customMs << " ms"
              << " | std::sort: " << std::setw(8) << std::fixed << std::setprecision(3) << stdMs << " ms\n";

    // 5. Correctness Sanity Check (Ensures performance optimization didn't break functionality)
    AssertVectorsEqual(datasetCustom, datasetStd);
}

INSTANTIATE_TEST_SUITE_P(
    ScaleRuns,
    TriangleSortPerfTest,
    ::testing::Values(100, 1000, 10000, 100000, 1000000)
);
