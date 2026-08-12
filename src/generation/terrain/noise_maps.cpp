#include "noise_maps.hpp"

#include <FastNoise/FastNoise.h>

#include "FastNoise/Generators/Cellular.h"
#include "FastNoise/Generators/DomainWarpSimplex.h"
#include "FastNoise/Generators/Fractal.h"
#include "FastNoise/Generators/Modifiers.h"
#include "FastNoise/Generators/Simplex.h"
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

    /* Best way to develop this kind of noise is to use Node Editor related to FastNoise2 (see their github repo).
     * Encoded node tree:  KAAC@BER4JKQkNAAU@BIpHD9PwkG@BlkMEA65H4T4L4XqUPwQTAACAvwwQAABgFUQYuB6FvgkWCiQIw/UoPwkuAAE@BJDQAH@BC@AIEAJBw@ABZEED0KV78YZmZmPwQDmpkZPwsAAIA/HAMAACBCBBM@B/BA==
     * For better understanding without the use of editor we are also providng the code to build the same node tree programmatically.
     * The structure should be the roughly the same, but the constants may differ.
     *
     * The idea behind this noise is to have some "continental" noise which wold determine where is land and then on top of it add
     * mountains.
     * */
    void TerrainNoiseMap::buildElevationNodeTree(bool useEncodedString)
    {
        if (useEncodedString)
        {
            elevationRootNode = FastNoise::NewFromEncodedNodeTree(config::generation::ELEVATION_NODE_TREE_ENCODED);
            return;
        }

        // CONTINENT
        auto continentalBase = FastNoise::New<FastNoise::Simplex>();
        continentalBase->SetScale(config::generation::CONTINENTAL_SCALE);

        auto continentalFractal = FastNoise::New<FastNoise::FractalFBm>();
        continentalFractal->SetSource(continentalBase);
        continentalFractal->SetWeightedStrength(1.16f);
        continentalFractal->SetOctaveCount(5);
        continentalFractal->SetLacunarity(2.0f);

        auto continentalSqrt = FastNoise::New<FastNoise::SignedSquareRoot>();
        continentalSqrt->SetSource(continentalFractal);
        // auto continentalSquared = FastNoise::New<FastNoise::Multiply>();
        // continentalSquared->SetLHS(continentalFractal);
        // continentalSquared->SetRHS(continentalFractal);
        //
        // auto continentalCubed = FastNoise::New<FastNoise::Multiply>();
        // continentalCubed->SetLHS(continentalSquared);
        // continentalCubed->SetRHS(continentalFractal);

        // MOUNTAINS
        auto mountainBase = FastNoise::New<FastNoise::SuperSimplex>();
        mountainBase->SetScale(config::generation::MOUNTAIN_SCALE);

        auto mountainFractal = FastNoise::New<FastNoise::FractalFBm>();
        mountainFractal->SetSource(mountainBase);
        mountainFractal->SetGain(0.5f);
        mountainFractal->SetOctaveCount(7);
        mountainFractal->SetWeightedStrength(1.0f);
        mountainFractal->SetLacunarity(2.5f);

        auto mountainRotatedPlane = FastNoise::New<FastNoise::DomainRotatePlane>();
        mountainRotatedPlane->SetSource(mountainFractal);
        mountainRotatedPlane->SetRotationType(FastNoise::PlaneRotationType::ImproveXZPlanes);

        auto mountainAxisScale = FastNoise::New<FastNoise::DomainAxisScale>();
        mountainAxisScale->SetSource(mountainRotatedPlane);
        mountainAxisScale->SetScaling<FastNoise::Dim::Y>(0.66f);

        auto mountainInvert = FastNoise::New<FastNoise::Subtract>();
        mountainInvert->SetRHS(mountainAxisScale);

        auto mountainWarpSimplex = FastNoise::New<FastNoise::DomainWarpSimplex>();
        mountainWarpSimplex->SetSource(mountainInvert);
        mountainWarpSimplex->SetWarpAmplitude(45.f);
        mountainWarpSimplex->SetScale(600.0f);
        mountainWarpSimplex->SetAmplitudeScaling<FastNoise::Dim::Y>(-0.26);
        mountainWarpSimplex->SetVectorizationScheme(FastNoise::VectorizationScheme::OrthogonalGradientMatrix);


        auto finalTerrain = FastNoise::New<FastNoise::Fade>();
        finalTerrain->SetA(continentalSqrt);
        finalTerrain->SetB(mountainWarpSimplex);
        finalTerrain->SetFade(0.0f);
        finalTerrain->SetFadeMax(0.2);

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

        temperatureRootNode = temperatureRemap;
    }
}
