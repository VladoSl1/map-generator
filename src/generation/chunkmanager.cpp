#include "chunkmanager.hpp"

#include "core/config.hpp"

#include "core/math/point2d.hpp"
#include "generation/pipeline/pipeline.hpp"
#include "generation/pipeline/point_sampling.hpp"

#include "core/utils/debug.hpp"

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

    void ChunkManager::addChunk(math::Point2Di chunkCoords)
    {
        uint64_t key = hashChunk(chunkCoords);

        if (chunks.find(key) != chunks.end()) return;


        std::vector<math::Point2Dd> combinedSeeds;
        combinedSeeds.reserve(config::generation::NUM_POINTS * 9);

        // since Point2Dd is a POD type, we can use insert is same as move
        // center chunk is added first, so that its easier to access
        auto centerSeeds = pipeline::samplePoints(hashChunk(chunkCoords), Chunk::getBounds(chunkCoords), config::generation::NUM_POINTS);
        combinedSeeds.insert(combinedSeeds.end(), centerSeeds.begin(), centerSeeds.end());


        for (int x = -1; x <= 1; ++x)
        {
            for (int y = -1; y <= 1; ++y)
            {
                if (x == 0 && y == 0) continue; // center chunk already added

                math::Point2Di neighborCoords{chunkCoords.x + x, chunkCoords.y + y};
                uint64_t neighborKey = hashChunk(neighborCoords);
                auto neighborSeeds = pipeline::samplePoints(neighborKey,
                                                             Chunk::getBounds(neighborCoords),
                                                             config::generation::NUM_POINTS);

                combinedSeeds.insert(combinedSeeds.end(), neighborSeeds.begin(), neighborSeeds.end());
            }
        }

        pipeline::VoronoiDiagram extendedDiagram = pipeline::generateFromPoints(combinedSeeds);
        std::vector<math::Point2Dd> relaxedPoints = extendedDiagram.seeds; // TODO: consider maybe leave empty

        for (int x = 0; x < config::generation::RELAXATION_ITERATIONS; ++x)
        {
            relaxedPoints = pipeline::relaxVoronoiDiagram(extendedDiagram);
            extendedDiagram = pipeline::generateFromPoints(relaxedPoints);
        }

        std::vector<math::Point2Dd> relaxedCenterPoints(
                relaxedPoints.begin(),
                relaxedPoints.begin() + config::generation::NUM_POINTS
            );

        Chunk newChunk(chunkCoords);
        newChunk.voronoiDiagram = pipeline::generateFromPoints(relaxedPoints);

        chunks.emplace(key, std::move(newChunk));
    }


    void ChunkManager::removeChunk(math::Point2Di chunkCoords)
    {

    }


    Chunk* ChunkManager::getChunk(math::Point2Di chunkCoords)
    {
        long key = hashChunk(chunkCoords);

        auto it = chunks.find(key);
        if (it != chunks.end())
        {
            return &(it->second);
        }
        else
        {
            return nullptr;
        }
    }

    void ChunkManager::preloadChunks(math::Point2Di centerChunkCoords)
    {
        // we are preloading 3x3 chunks around the center chunk
        // for (int x = -1; x <= 1; ++x)
        // {
        //     for (int y = -1; y <= 1; ++y)
        //     {
        //         addChunk({centerChunkCoords.x + x, centerChunkCoords.y + y});
        //     }
        // }
    }

    std::vector<const Chunk*> ChunkManager::listAllChunks() const
    {
        std::vector<const Chunk*> chunkList;
        for (const auto& pair : chunks)
        {
            chunkList.push_back(&(pair.second));
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
