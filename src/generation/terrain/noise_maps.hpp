#pragma once

#include <FastNoise/FastNoise.h>

#include <vector>

namespace terrain
{
    class TerrainNoiseMap
    {
        /* Arbitrary offset so that noise maps are independent */
        static constexpr int MOISTURE_SEED_OFFSET = 1171;
        static constexpr int TEMPERATURE_SEED_OFFSET = 31337;

    public:
        explicit TerrainNoiseMap(int seed);

        std::vector<float> generateElevationNoiseMap(const std::vector<float>& xCoords,
                                                     const std::vector<float>& yCoords) const;

        std::vector<float> generateMoistureNoiseMap(const std::vector<float>& xCoords,
                                                    const std::vector<float>& yCoords) const;

        std::vector<float> generateTemperatureNoiseMap(const std::vector<float>& xCoords,
                                                       const std::vector<float>& yCoords) const;
    private:
        int seed;
        FastNoise::SmartNode<> elevationRootNode;
        FastNoise::SmartNode<> moistureRootNode;
        FastNoise::SmartNode<> temperatureRootNode;
        FastNoise::SmartNode<> continentalNode;

        void buildElevationNodeTree();
        void buildMoistureNodeTree();
        void buildTemperatureNodeTree();


        std::vector<float> generateNoiseMap(const std::vector<float>& xCoords,
                                            const std::vector<float>& yCoords,
                                            const FastNoise::SmartNode<>& rootNode,
                                            int seedOffset = 0) const;
    };

}
