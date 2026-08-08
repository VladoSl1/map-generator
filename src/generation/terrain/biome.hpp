#pragma once

#include <cstdint>

namespace terrain
{

    // based on https://en.wikipedia.org/wiki/Biome#Whittaker_(1962,_1970,_1975)_biome-types
    enum class BiomeType : uint8_t
    {
        DEEP_OCEAN,
        OCEAN,
        BEACH,
        SCORCHED,
        BARE,
        TUNDRA,
        SNOW,
        TEMPERATE_DESERT,
        SHRUBLAND,
        TAIGA,
        SUBTROPICAL_DESERT,
        GRASSLAND,
        TROPICAL_SEASONAL_FOREST,
        TROPICAL_RAIN_FOREST
    };

    static BiomeType getBiomeType(float elevation, float moisture)
    {
        // Water / Coast
        if (elevation < -0.25f) return BiomeType::DEEP_OCEAN;
        if (elevation < 0.0f)   return BiomeType::OCEAN;
        if (elevation < 0.05f)  return BiomeType::BEACH;

        // High elevation (Mountains)
        if (elevation > 0.7f)
        {
            if (moisture < -0.5f) return BiomeType::SCORCHED;
            if (moisture < 0.0f)  return BiomeType::BARE;
            if (moisture < 0.5f)  return BiomeType::TUNDRA;
            return BiomeType::SNOW;
        }

        // Mid elevation (Hills / Highlands)
        if (elevation > 0.35f)
        {
            if (moisture < -0.33f) return BiomeType::TEMPERATE_DESERT;
            if (moisture < 0.33f)  return BiomeType::SHRUBLAND;
            return BiomeType::TAIGA;
        }

        // Low elevation (Plains)
        if (moisture < -0.5f) return BiomeType::SUBTROPICAL_DESERT;
        if (moisture < -0.1f) return BiomeType::GRASSLAND;
        if (moisture < 0.4f)  return BiomeType::TROPICAL_SEASONAL_FOREST;

        return BiomeType::TROPICAL_RAIN_FOREST;
    }

}
