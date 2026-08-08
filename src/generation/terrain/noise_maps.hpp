#pragma once

#include <FastNoise/FastNoise.h>

#include <vector>

namespace terrain
{
    class TerrainNoiseMap
    {
        /* Arbitrary offset so that moisture and elevation maps are independent */
        static constexpr int MOISTURE_SEED_OFFSET = 1171;

    public:
        explicit TerrainNoiseMap(int seed);

        std::vector<float> generateElevationNoiseMap(const std::vector<float>& xCoords,
                                                     const std::vector<float>& yCoords) const;

        std::vector<float> generateMoistureNoiseMap(const std::vector<float>& xCoords,
                                                    const std::vector<float>& yCoords) const;

    private:
        int seed;
        FastNoise::SmartNode<> elevationRootNode;
        FastNoise::SmartNode<> moistureRootNode;

        void buildElevationNodeTree();
        void buildMoistureNodeTree();

        std::vector<float> generateNoiseMap(const std::vector<float>& xCoords,
                                            const std::vector<float>& yCoords,
                                            const FastNoise::SmartNode<>& rootNode,
                                            int seedOffset = 0) const;
    };

}
