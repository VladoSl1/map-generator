#pragma once

#include "pipeline/voronoi_diagram.hpp"
#include "core/math/point2d.hpp"

#include <unordered_map>


namespace generation
{
    struct Chunk
    {
        math::Point2Di chunkCoords;

        pipeline::VoronoiDiagram voronoiDiagram;
    };

    class ChunkManager
    {

    public:

        ChunkManager(int worldSeed)
            : m_worldSeed(worldSeed)
        {

        }

        void addChunk(math::Point2Di chunkCoords);
        void removeChunk(math::Point2Di chunkCoords);


    private:
        uint64_t m_worldSeed;
        std::unordered_map<long, Chunk> chunks;

        Chunk* getChunk(math::Point2Di chunkCoords);

        /* We are using uint64_t to make this program platform independent
         * */
        uint64_t hashChunk(math::Point2Di chunkCoords) const;
    };

}
