#include "chunkmanager.hpp"

#include "core/config.hpp"

#include "core/math/point2d.hpp"
#include "generation/pipeline/pipeline.hpp"
#include "generation/pipeline/point_sampling.hpp"

#include "core/utils/debug.hpp"
#include <cstdint>

namespace generation
{
    Chunk::Chunk(math::Point2Di chunkCoords)
        : chunkCoords(chunkCoords)
    {
        bounds = getBounds(chunkCoords);
    }

    math::AABB Chunk::getBounds(math::Point2Di chunkCoords)
    {
        double minX = (static_cast<double>(chunkCoords.x) + 0.5) * config::generation::CHUNK_WIDTH;
        double minY = (static_cast<double>(chunkCoords.y) + 0.5) * config::generation::CHUNK_HEIGHT;
        return { {minX, minX + config::generation::CHUNK_WIDTH}, {minY, minY + config::generation::CHUNK_HEIGHT} };
    }

    void ChunkManager::loadChunk(math::Point2Di chunkCoords)
    {
        uint64_t key = hashChunk(chunkCoords);

        if (requestChunk(chunkCoords)->state == Chunk::State::LOADED)
        {
            return;
        }

        std::vector<math::Point2Dd> combinedSeeds;
        combinedSeeds.reserve(config::generation::NUM_POINTS * 9);

        preloadChunks(chunkCoords);

        for (int x = -1; x <= 1; ++x)
        {
            for (int y = -1; y <= 1; ++y)
            {
                math::Point2Di neighborChunkCoords = {chunkCoords.x + x, chunkCoords.y + y};
                std::shared_ptr<Chunk> neighborChunk = getChunk(neighborChunkCoords);

                // since Point2Dd is a POD type, we can use insert is same as move
                if (neighborChunk->state == Chunk::State::PRELOADED)
                {
                    combinedSeeds.insert(combinedSeeds.end(), neighborChunk->preloadedSeeds.begin(), neighborChunk->preloadedSeeds.end());
                }
                else if (neighborChunk->state == Chunk::State::LOADED)
                {
                    combinedSeeds.insert(combinedSeeds.end(), neighborChunk->voronoiDiagram.seeds.begin(), neighborChunk->voronoiDiagram.seeds.end());
                }
            }
        }


        pipeline::VoronoiDiagram extendedDiagram = pipeline::generateFromPoints(combinedSeeds);
        std::vector<math::Point2Dd> relaxedPoints = extendedDiagram.seeds; // TODO: consider maybe leave empty

        for (int x = 0; x < config::generation::RELAXATION_ITERATIONS; ++x)
        {
            relaxedPoints = pipeline::relaxVoronoiDiagram(extendedDiagram);
            extendedDiagram = pipeline::generateFromPoints(relaxedPoints);
        }

        // center chunk is at the center of 3x3 grid -> offset is 4 Chunks
        std::vector<math::Point2Dd> relaxedCenterPoints(
                relaxedPoints.begin() + config::generation::NUM_POINTS * 4,
                relaxedPoints.begin() + config::generation::NUM_POINTS * 5);

        auto centerChunk = requestChunk(chunkCoords);
        centerChunk->voronoiDiagram = pipeline::generateFromPoints(relaxedCenterPoints);

        log("Loaded chunk at coords: (" + std::to_string(chunkCoords.x) + ", " + std::to_string(chunkCoords.y) + ")");
        for (int i = 0; i < centerChunk->voronoiDiagram.seeds.size(); ++i)
        {
            log("Seed " + std::to_string(i) + ": (" + std::to_string(centerChunk->voronoiDiagram.seeds[i].x) + ", " + std::to_string(centerChunk->voronoiDiagram.seeds[i].y) + ")");
        }

        chunks.emplace(key, std::move(centerChunk));
        centerChunk->preloadedSeeds.clear();
        centerChunk->state = Chunk::State::LOADED;
    }


    void ChunkManager::removeChunk(math::Point2Di chunkCoords)
    {
        //TODO:
    }


    std::shared_ptr<Chunk> ChunkManager::addChunk(math::Point2Di chunkCoords)
    {
        std::shared_ptr<Chunk> newChunk = std::make_shared<Chunk>(chunkCoords);
        chunks.emplace(hashChunk(chunkCoords), newChunk);

        return newChunk;
    }

    std::shared_ptr<Chunk> ChunkManager::getChunk(math::Point2Di chunkCoords) const
    {
        uint64_t key = hashChunk(chunkCoords);

        auto it = chunks.find(key);
        if (it != chunks.end())
        {
            return it->second;
        }
        else
        {
            return nullptr;
        }
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

        chunk->preloadedSeeds = pipeline::samplePoints(hashChunk(chunkCoords),
                                                       Chunk::getBounds(chunkCoords),
                                                       config::generation::NUM_POINTS);
        chunk->state = Chunk::State::PRELOADED;
    }


    void ChunkManager::preloadChunks(math::Point2Di centerChunkCoords)
    {
        // we are preloading 3x3 chunks around the center chunk
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
        for (const auto& pair : chunks)
        {
            chunkList.push_back(pair.second);
        }

        return chunkList;
    }

    uint64_t ChunkManager::hashChunk(math::Point2Di chunkCoords) const
    {
        // cast to uint32_t first to drop the sign-extension behavior
        uint32_t ux = static_cast<uint32_t>(chunkCoords.x);
        uint32_t uy = static_cast<uint32_t>(chunkCoords.y);

        uint64_t seed = (static_cast<uint64_t>(ux) << 32) | uy;

        seed ^= m_worldSeed;

        // alg: https://rosettacode.org/wiki/Pseudo-random_numbers/Splitmix64
        seed += 0x9e3779b97f4a7c15;
        seed = (seed ^ (seed >> 30)) * 0xbf58476d1ce4e5b9;
        seed = (seed ^ (seed >> 27)) * 0x94d049bb133111eb;
        return seed ^ (seed >> 31);
    }
}
