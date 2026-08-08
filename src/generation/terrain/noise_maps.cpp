#include "noise_maps.hpp"

#include <FastNoise/FastNoise.h>

#include "FastNoise/Generators/Fractal.h"
#include "core/config.hpp"

#include <vector>


namespace terrain
{

    TerrainNoiseMap::TerrainNoiseMap(int seed)
        : seed(seed), elevationRootNode(nullptr), moistureRootNode(nullptr), temperatureRootNode(nullptr)
    {
        buildElevationNodeTree();
        buildMoistureNodeTree();
        buildTemperatureNodeTree();
    }

    std::vector<float> TerrainNoiseMap::generateElevationNoiseMap(const std::vector<float>& xCoords,
                                                                  const std::vector<float>& yCoords) const
    {
        return generateNoiseMap(xCoords, yCoords, elevationRootNode);
    }

    std::vector<float> TerrainNoiseMap::generateMoistureNoiseMap(const std::vector<float>& xCoords,
                                                                 const std::vector<float>& yCoords) const
    {
        return generateNoiseMap(xCoords, yCoords, moistureRootNode, MOISTURE_SEED_OFFSET);
    }

    std::vector<float> TerrainNoiseMap::generateTemperatureNoiseMap(const std::vector<float>& xCoords,
                                                                    const std::vector<float>& yCoords) const
    {
        return generateNoiseMap(xCoords, yCoords, temperatureRootNode, TEMPERATURE_SEED_OFFSET);
    }

    std::vector<float> TerrainNoiseMap::generateNoiseMap(const std::vector<float>& xCoords,
                                                         const std::vector<float>& yCoords,
                                                         const FastNoise::SmartNode<>& rootNode,
                                                         int seedOffset) const
    {
        assert(rootNode != nullptr && "Root node must be initialized before generating noise map");

        std::vector<float> noiseMap(xCoords.size());

        rootNode->GenPositionArray2D(noiseMap.data(),
                                     static_cast<int>(xCoords.size()),
                                     xCoords.data(),
                                     yCoords.data(),
                                     0.0f,  // these offsets are used for shifting sampling region
                                     0.0f,  // the x and y coordinates provided are in the world space, so we don't need to offset them
                                     seed + seedOffset);

        return std::move(noiseMap);
    }


    void TerrainNoiseMap::buildElevationNodeTree()
    {
        // large continents and oceans
        auto continentalBase = FastNoise::New<FastNoise::Simplex>();
        continentalBase->SetScale(config::generation::CONTINENTAL_SCALE);

        auto continentalFractal = FastNoise::New<FastNoise::FractalFBm>();
        continentalFractal->SetSource(continentalBase);
        continentalFractal->SetOctaveCount(20);

        auto erosionBase = FastNoise::New<FastNoise::Simplex>();
        erosionBase->SetScale(config::generation::EROSION_SCALE);

        // scales the level of Detail in the noise map, flat vs. steep
        auto erosionRemap = FastNoise::New<FastNoise::Remap>();
        erosionRemap->SetSource(erosionBase);
        // mapping [-1, 1] to [0, 1] so that the erosion noise can be used as a weight for the detail noise
        erosionRemap->SetFromMin(-1.0f);
        erosionRemap->SetFromMax(1.0f);
        erosionRemap->SetToMin(0.0f);
        erosionRemap->SetToMax(0.25f);

        // detail
        auto detailBase = FastNoise::New<FastNoise::Simplex>();
        detailBase->SetScale(config::generation::DETAIL_SCALE);

        auto detailMask = FastNoise::New<FastNoise::FractalFBm>();
        detailMask->SetSource(detailBase);
        detailMask->SetOctaveCount(config::generation::DETAIL_OCTAVE_COUNT);

        auto modulateDetail = FastNoise::New<FastNoise::Multiply>();
        modulateDetail->SetLHS(detailMask);
        modulateDetail->SetRHS(erosionRemap);

        auto coastBase = FastNoise::New<FastNoise::Abs>();
        coastBase->SetSource(continentalBase);

        auto coastRemap = FastNoise::New<FastNoise::Remap>();
        coastRemap->SetSource(coastBase);
        coastRemap->SetFromMin(0.0f);
        coastRemap->SetFromMax(0.5f); // Tune this to make coastlines wider or tighter
        coastRemap->SetToMin(0.0f);    // 0 detail exactly at coast
        coastRemap->SetToMax(1.0f);    // 1.0 multiplier inland/deep sea

        auto finalDetailMask = FastNoise::New<FastNoise::Multiply>();
        finalDetailMask->SetLHS(modulateDetail);
        finalDetailMask->SetRHS(coastRemap);

        auto finalTerrain = FastNoise::New<FastNoise::Add>();
        finalTerrain->SetLHS(continentalFractal);
        // finalTerrain->SetRHS(modulateDetail);
        finalTerrain->SetRHS(finalDetailMask);

        elevationRootNode = finalTerrain;
    }

    void TerrainNoiseMap::buildMoistureNodeTree()
    {
        auto moistureBase = FastNoise::New<FastNoise::Simplex>();
        moistureBase->SetScale(config::generation::MOISTURE_SCALE);

        auto moistureFractal = FastNoise::New<FastNoise::FractalFBm>();
        moistureFractal->SetSource(moistureBase);
        moistureFractal->SetOctaveCount(config::generation::MOISTURE_OCTAVE_COUNT);



        moistureRootNode = moistureFractal;
    }

    void TerrainNoiseMap::buildTemperatureNodeTree()
    {
        auto temperatureBase = FastNoise::New<FastNoise::Simplex>();
        temperatureBase->SetScale(config::generation::TEMPERATURE_SCALE);

        auto temperatureFractal = FastNoise::New<FastNoise::FractalRidged>();
        temperatureFractal->SetSource(temperatureBase);
        temperatureFractal->SetOctaveCount(config::generation::TEMPERATURE_OCTAVE_COUNT);

        auto temperatureRemap = FastNoise::New<FastNoise::Remap>();
        temperatureRemap->SetSource(temperatureFractal);
        temperatureRemap->SetFromMin(-1.0f);
        temperatureRemap->SetFromMax(1.0f);
        temperatureRemap->SetToMin(0.0f);
        temperatureRemap->SetToMax(1.0f);

        temperatureRootNode = temperatureBase;
    }
}
