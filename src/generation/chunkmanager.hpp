#pragma once

#include "pipeline/voronoi_diagram.hpp"
#include "terrain/noise_maps.hpp"
#include "terrain/biome.hpp"

#include "core/math/point2d.hpp"
#include "core/math/aabb.hpp"
#include "core/config.hpp"

#include <unordered_map>
#include <vector>
#include <memory>

namespace generation
{

    struct TerrainData
    {
        std::vector<float> elevationMap;
        std::vector<float> moistureMap;
        std::vector<float> temperatureMap;
        std::vector<terrain::BiomeType> biomes;
    };

    struct Chunk
    {
        enum class State : uint8_t
        {
            UNLOADED,  // only instance of chunk was created with initialized state variables
            PRELOADED, // only seeds are generated
            LOADED     // chunk is fully initialized
        };

        math::Point2Di chunkCoords;
        math::AABB bounds;

        std::vector<math::Point2Dd> preloadedSeeds;
        pipeline::VoronoiDiagram voronoiDiagram;

        TerrainData terrainData;

        State state = State::UNLOADED;

        explicit Chunk(math::Point2Di chunkCoords);

        static math::AABB getBounds(math::Point2Di chunkCoords);
    };

    class ChunkManager
    {
    public:
        explicit ChunkManager(int worldSeed);

        void loadChunk(math::Point2Di chunkCoords);
        void removeChunk(math::Point2Di chunkCoords);

        std::shared_ptr<Chunk> addChunk(math::Point2Di chunkCoords);
        std::shared_ptr<Chunk> getChunk(math::Point2Di chunkCoords) const;
        std::shared_ptr<Chunk> requestChunk(math::Point2Di chunkCoords);

        /* Fill chunks only with seed points */
        void preloadChunk(math::Point2Di chunkCoords);
        void preloadChunks(math::Point2Di centerChunkCoords);
        std::vector<std::shared_ptr<Chunk>> listAllChunks() const;

        static math::Point2Di worldToChunkCoords(math::Point2Dd worldCoords)
        {
            return {
                static_cast<int>(std::floor(worldCoords.x / config::generation::CHUNK_WIDTH)),
                static_cast<int>(std::floor(worldCoords.y / config::generation::CHUNK_HEIGHT))
            };
        }


    private:
        uint64_t m_worldSeed;
        std::unordered_map<uint64_t, std::shared_ptr<Chunk>> chunks; // TODO: consider using unique_ptr

        terrain::TerrainNoiseMap terrainNoiseMap;

        /* We are using uint64_t to have guaranteed 64-bit hash values */
        uint64_t hashChunk(math::Point2Di chunkCoords) const;
    };
}
