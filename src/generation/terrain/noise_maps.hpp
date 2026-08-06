#pragma once

#include <FastNoise/FastNoise.h>

#include <vector>

namespace terrain
{

    class TerrainNoiseMap
    {
    public:
        explicit TerrainNoiseMap(int seed);

        std::vector<float> getElevationNoiseMap(const std::vector<float>& xCoords,
                                                const std::vector<float>& yCoords) const;

    private:
        int seed;
        FastNoise::SmartNode<> rootNode;

        void BuildNodeTree();

    };

}
