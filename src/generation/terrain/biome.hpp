#pragma once

#include <cstdint>
#include <algorithm>

namespace terrain
{
    /* based on https://en.wikipedia.org/wiki/Biome#Whittaker_(1962,_1970,_1975)_biome-types */
    enum class BiomeType : uint8_t
    {
        DEEP_OCEAN,
        OCEAN,
        LAKE,
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

    // TODO: this neeeds to be tweaked
    static BiomeType getBiomeType(float elevation, float moisture, float baseTemperature)
    {
        if (elevation < -0.35f) return BiomeType::DEEP_OCEAN; // Pushed deeper
        if (elevation < 0.0f)   return BiomeType::OCEAN;

        if (elevation > 0.0f && elevation < 0.15f && moisture > 0.5f) return BiomeType::LAKE;
        if (elevation < 0.05f)  return BiomeType::BEACH;

        float temperature = baseTemperature - std::max(0.0f, elevation * 0.4f);

        // Solidify mountain/peak generation at high elevations
        if (elevation > 0.65f)
        {
            if (temperature < -0.1f || moisture > 0.3f) return BiomeType::SNOW;
            return BiomeType::BARE;
        }

        if (temperature > 0.15f) // Tropical (Warm)
        {
            if (moisture < -0.3f) return BiomeType::SUBTROPICAL_DESERT;
            if (moisture < 0.0f)  return BiomeType::GRASSLAND;
            if (moisture < 0.4f)  return BiomeType::TROPICAL_SEASONAL_FOREST;
            return BiomeType::TROPICAL_RAIN_FOREST;
        }
        else if (temperature > -0.25f) // Temperate (Mid)
        {
            if (moisture < -0.3f) return BiomeType::TEMPERATE_DESERT;
            if (moisture < 0.0f)  return BiomeType::SHRUBLAND;
            return BiomeType::TAIGA;
        }
        else // Cold (High peaks or polar regions)
        {
            if (moisture < -0.3f) return BiomeType::SCORCHED;
            if (moisture < 0.0f)  return BiomeType::BARE;
            if (moisture < 0.4f)  return BiomeType::TUNDRA;
            return BiomeType::SNOW;
        }
    }

}
