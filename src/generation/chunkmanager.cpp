#include "chunkmanager.hpp"

#include "generation/pipeline/pipeline.hpp"
#include "generation/pipeline/point_sampling.hpp"
#include "generation/pipeline/voronoi_diagram.hpp"
#include "generation/terrain/noise_maps.hpp"
#include "generation/terrain/biome.hpp"

#include "core/config.hpp"
#include "core/math/point2d.hpp"
#include "core/utils/debug.hpp"

#include <cstdint>

namespace generation
{
    Chunk::Chunk(math::Point2Di chunkCoords)
        : chunkCoords(chunkCoords), bounds(getBounds(chunkCoords))
    {
    }

    // TODO: consider moving this somewhere else for better encapsulation
    math::AABB Chunk::getBounds(math::Point2Di chunkCoords)
    {
        double minX = (static_cast<double>(chunkCoords.x) + 0.0) * config::generation::CHUNK_WIDTH;
        double minY = (static_cast<double>(chunkCoords.y) + 0.0) * config::generation::CHUNK_HEIGHT;
        return { {minX, minX + config::generation::CHUNK_WIDTH}, {minY, minY + config::generation::CHUNK_HEIGHT} };
    }

    ChunkManager::ChunkManager(int worldSeed)
        : m_worldSeed(worldSeed), terrainNoiseMap(worldSeed)
    {
        preloadChunks({0, 0});
    }


    /*
    * First, preload chunks (generate only seeds). Then copy all of these seeds to combinedSeeds
    * and generate large voronoi diagram for 3x3 chunks. Relaxing this large diagram ensures that
    * even the voronoi cells on the edges of central chunk are relaxed properly.
    * The relaxation is happening on unrelaxed seeds of neighboring chunks, therefore the final chunk
    * will be always the same regardless of the order of loading chunks.
    */
    void ChunkManager::loadChunk(math::Point2Di chunkCoords)
    {
        const uint64_t key = hashChunk(chunkCoords);

        auto centerChunk = requestChunk(chunkCoords);
        if (centerChunk->state == Chunk::State::LOADED)
        {
            return;
        }

        std::vector<math::Point2Dd> combinedSeeds;
        combinedSeeds.reserve(9L * config::generation::NUM_POINTS);  // 9 chunks, each with NUM_POINTS seeds

        preloadChunks(chunkCoords);

        for (int x = -1; x <= 1; ++x)
        {
            for (int y = -1; y <= 1; ++y)
            {
                math::Point2Di neighborChunkCoords = {chunkCoords.x + x, chunkCoords.y + y};
                std::shared_ptr<Chunk> neighborChunk = getChunk(neighborChunkCoords);
                combinedSeeds.insert(combinedSeeds.end(), neighborChunk->preloadedSeeds.begin(), neighborChunk->preloadedSeeds.end());
            }
        }

        startClock("Generating Voronoi diagram for chunk " + std::to_string(chunkCoords.x) + ", " + std::to_string(chunkCoords.y));

        pipeline::DynamicVoronoiDiagram extendedDiagram = pipeline::generateFromPoints(combinedSeeds);
        for (int i = 0; i < config::generation::RELAXATION_ITERATIONS; ++i)
        {
            extendedDiagram.relax(false);
        }

        stopClock("Generating Voronoi diagram for chunk " + std::to_string(chunkCoords.x) + ", " + std::to_string(chunkCoords.y));

        const size_t startIndex = 4L*config::generation::NUM_POINTS;  // central chunk is at index 4 in the 3x3 grid of chunks
        const size_t endIndex = startIndex + config::generation::NUM_POINTS;
        centerChunk->voronoiDiagram = extendedDiagram.extractVoronoiSubset(startIndex, endIndex);

        std::vector<float> xCoords;
        std::vector<float> yCoords;
        xCoords.reserve(centerChunk->voronoiDiagram.seeds.size());
        yCoords.reserve(centerChunk->voronoiDiagram.seeds.size());

        for (const auto& seed : centerChunk->voronoiDiagram.seeds)
        {
            xCoords.push_back(static_cast<float>(seed.x));
            yCoords.push_back(static_cast<float>(seed.y));
        }

        auto& terrain = centerChunk->terrainData;
        terrain.elevationMap = terrainNoiseMap.generateElevationNoiseMap(xCoords, yCoords);
        terrain.moistureMap = terrainNoiseMap.generateMoistureNoiseMap(xCoords, yCoords);
        terrain.temperatureMap = terrainNoiseMap.generateTemperatureNoiseMap(xCoords, yCoords);
        terrain.biomes.reserve(xCoords.size());

        for (size_t i = 0; i < xCoords.size(); ++i)
        {
            terrain.biomes.push_back(
                terrain::getBiomeType(terrain.elevationMap[i], terrain.moistureMap[i], terrain.temperatureMap[i])
            );
        }

        centerChunk->state = Chunk::State::LOADED;
    }

    void ChunkManager::removeChunk(math::Point2Di chunkCoords)
    {
        // TODO:consider maybe storing chunks on disc instead of just recalculating them
        chunks.erase(hashChunk(chunkCoords));
    }

    std::shared_ptr<Chunk> ChunkManager::addChunk(math::Point2Di chunkCoords)
    {
        std::shared_ptr<Chunk> newChunk = std::make_shared<Chunk>(chunkCoords);
        chunks.emplace(hashChunk(chunkCoords), newChunk);
        return newChunk;
    }

    std::shared_ptr<Chunk> ChunkManager::getChunk(math::Point2Di chunkCoords) const
    {
        const uint64_t key = hashChunk(chunkCoords);
        auto it = chunks.find(key);
        if (it != chunks.end())
        {
            return it->second;
        }
        return nullptr;
    }

    std::shared_ptr<Chunk> ChunkManager::requestChunk(math::Point2Di chunkCoords)
    {
        auto chunk = getChunk(chunkCoords);
        if (chunk == nullptr)
        {
            chunk = addChunk(chunkCoords);
        }
        return chunk;
    }

    void ChunkManager::preloadChunk(math::Point2Di chunkCoords)
    {
        auto chunk = requestChunk(chunkCoords);
        if (chunk->state != Chunk::State::UNLOADED)
        {
            return;
        }

        chunk->preloadedSeeds = pipeline::samplePoints(
            hashChunk(chunkCoords),
            Chunk::getBounds(chunkCoords),
            config::generation::NUM_POINTS
        );
        chunk->state = Chunk::State::PRELOADED;
    }

    void ChunkManager::preloadChunks(math::Point2Di centerChunkCoords)
    {
        // preload 3x3 grid of chunks around the center chunk
        for (int x = -1; x <= 1; ++x)
        {
            for (int y = -1; y <= 1; ++y)
            {
                preloadChunk({centerChunkCoords.x + x, centerChunkCoords.y + y});
            }
        }
    }

    std::vector<std::shared_ptr<Chunk>> ChunkManager::listAllChunks() const
    {
        std::vector<std::shared_ptr<Chunk>> chunkList;
        chunkList.reserve(chunks.size());

        for (const auto& pair : chunks)
        {
            chunkList.push_back(pair.second);
        }
        return chunkList;
    }

    uint64_t ChunkManager::hashChunk(math::Point2Di chunkCoords) const
    {
        // cast to uint32_t first to drop the sign-extension behavior
        auto ux = static_cast<uint32_t>(chunkCoords.x);
        auto uy = static_cast<uint32_t>(chunkCoords.y);

        uint64_t seed = (static_cast<uint64_t>(ux) << 32) | uy;
        seed ^= m_worldSeed;

        // alg: https://rosettacode.org/wiki/Pseudo-random_numbers/Splitmix64
        seed += 0x9e3779b97f4a7c15;
        seed = (seed ^ (seed >> 30)) * 0xbf58476d1ce4e5b9;
        seed = (seed ^ (seed >> 27)) * 0x94d049bb133111eb;
        return seed ^ (seed >> 31);
    }

}
