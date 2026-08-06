#include "noise_maps.hpp"

#include <FastNoise/FastNoise.h>

#include "FastNoise/Generators/Fractal.h"
#include "core/config.hpp"


#include <vector>

namespace terrain
{

    TerrainNoiseMap::TerrainNoiseMap(int seed)
        : rootNode(nullptr), seed(seed)
    {
        BuildNodeTree();
    }

    std::vector<float> TerrainNoiseMap::getElevationNoiseMap(const std::vector<float>& xCoords,
                                                             const std::vector<float>& yCoords) const
    {
        std::vector<float> noiseMap;
        noiseMap.reserve(xCoords.size());

        rootNode->GenPositionArray2D(noiseMap.data(),
                                     xCoords.size(),
                                     xCoords.data(),
                                     yCoords.data(),
                                     0.0f,  // these offsets are used for shifting sampling region
                                     0.0f,  // this function expects the x and y coordinates to be in world space, so we don't need to offset them
                                     seed);

    }

    void TerrainNoiseMap::BuildNodeTree()
    {
        // large continents and oceans
        auto continentalBase = FastNoise::New<FastNoise::Simplex>();
        continentalBase->SetScale(config::generation::CONTINENTAL_SCALE);

        auto erosionBase = FastNoise::New<FastNoise::Simplex>();
        erosionBase->SetScale(config::generation::EROSION_SCALE);

        // scales the level of Detail in the noise map, flat vs. steep
        auto erosionRemap = FastNoise::New<FastNoise::Remap>();
        erosionRemap->SetSource(erosionBase);
        // mapping [-1, 1] to [0, 1] so that the erosion noise can be used as a weight for the detail noise
        erosionRemap->SetFromMin(-1.0f);
        erosionRemap->SetFromMax(1.0f);
        erosionRemap->SetToMin(0.0f);
        erosionRemap->SetToMax(1.0f);

        // detail
        auto detailBase = FastNoise::New<FastNoise::Simplex>();
        detailBase->SetScale(config::generation::DETAIL_SCALE);

        auto detailRemap = FastNoise::New<FastNoise::FractalFBm>();
        detailRemap->SetSource(detailBase);
        detailRemap->SetOctaveCount(config::generation::DETAIL_OCTAVE_COUNT);

        auto modulateDetail = FastNoise::New<FastNoise::Multiply>();
        modulateDetail->SetLHS(detailRemap);
        modulateDetail->SetRHS(erosionRemap);

        auto finalTerrain = FastNoise::New<FastNoise::Add>();
        finalTerrain->SetLHS(continentalBase);
        finalTerrain->SetRHS(modulateDetail);

        rootNode = finalTerrain;
    }
}
