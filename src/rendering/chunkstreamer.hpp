#pragma once

#include "generation/chunkmanager.hpp"

#include "core/math/aabb.hpp"


namespace renderer
{

    class ChunkStreamer
    {
    public:
        ChunkStreamer(std::shared_ptr<generation::ChunkManager> chunkManager);

        void updateLoadedChunks(const math::AABB& viewBounds);

    private:
        std::shared_ptr<generation::ChunkManager> chunkManager;


    };


}
