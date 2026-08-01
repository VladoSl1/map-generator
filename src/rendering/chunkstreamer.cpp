#include "chunkstreamer.hpp"

#include "generation/chunkmanager.hpp"
#include "core/config.hpp"

#include "core/math/point2d.hpp"
#include "core/math/aabb.hpp"
#include "core/utils/debug.hpp"

#include <memory>
#include <utility>

namespace renderer
{

    ChunkStreamer::ChunkStreamer(std::shared_ptr<generation::ChunkManager> chunkManager)
        : m_chunkManager(std::move(chunkManager))
    {
    }

    void ChunkStreamer::updateLoadedChunks(const math::AABB& viewBounds)
    {
        const math::Point2Di topLeft = generation::ChunkManager::worldToChunkCoords({viewBounds.x_bounds.min, viewBounds.y_bounds.min});
        const math::Point2Di bottomRight  = generation::ChunkManager::worldToChunkCoords({viewBounds.x_bounds.max, viewBounds.y_bounds.max});

        // TODO: this is very slow, reason why?

        // auto allChunks = m_chunkManager->listAllChunks();
        // for (const auto& chunk : allChunks)
        // {
        //     math::Point2Di chunkCoords = chunk->chunkCoords;
        //     if (chunkCoords.x < topLeft.x || chunkCoords.x > bottomRight.y ||
        //         chunkCoords.y < bottomRight.y || chunkCoords.y > topLeft.y)
        //     {
        //         m_chunkManager->removeChunk(chunkCoords);
        //     }
        // }
        //

        for (int x = topLeft.x - config::renderer::CHUNK_RENDER_DISTANCE; x <= bottomRight.x + config::renderer::CHUNK_RENDER_DISTANCE; ++x)
        {
            for (int y = topLeft.y - config::renderer::CHUNK_RENDER_DISTANCE; y <= bottomRight.y + config::renderer::CHUNK_RENDER_DISTANCE; ++y)
            {
                m_chunkManager->loadChunk({x, y});
            }
        }
    }
}
