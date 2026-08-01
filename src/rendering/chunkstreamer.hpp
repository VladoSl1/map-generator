#pragma once

#include "generation/chunkmanager.hpp"

#include "core/math/aabb.hpp"

#include <memory>


namespace renderer
{

    class ChunkStreamer
    {
    public:
        explicit ChunkStreamer(std::shared_ptr<generation::ChunkManager> chunkManager);

        void updateLoadedChunks(const math::AABB& viewBounds);

    private:
        std::shared_ptr<generation::ChunkManager> m_chunkManager;


    };


}
