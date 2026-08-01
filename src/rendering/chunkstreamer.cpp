#include "chunkstreamer.hpp"
#include "core/math/constants.hpp"

#include "core/utils/debug.hpp"

namespace renderer
{

    ChunkStreamer::ChunkStreamer(std::shared_ptr<generation::ChunkManager> chunkManager)
        : chunkManager(chunkManager)
    {
    }

    void ChunkStreamer::updateLoadedChunks(const math::AABB& viewBounds)
    {
        math::Point2Di topLeft = generation::ChunkManager::worldToChunkCoords({viewBounds.x_bounds.min, viewBounds.y_bounds.min});
        math::Point2Di bottomRight  = generation::ChunkManager::worldToChunkCoords({viewBounds.x_bounds.max, viewBounds.y_bounds.max});

        log(viewBounds.x_bounds.min, " ", viewBounds.x_bounds.max, " ", viewBounds.y_bounds.min, " ", viewBounds.y_bounds.max, " | ");
        log(topLeft.x, " ", topLeft.y, " ", bottomRight.x, " ", bottomRight.y);

        log(topLeft.x - config::renderer::CHUNK_RENDER_DISTANCE, " ", topLeft.y - config::renderer::CHUNK_RENDER_DISTANCE, " ", bottomRight.x + config::renderer::CHUNK_RENDER_DISTANCE, " ", bottomRight.y + config::renderer::CHUNK_RENDER_DISTANCE);

        log("\n");


        for (int x = topLeft.x - config::renderer::CHUNK_RENDER_DISTANCE; x <= bottomRight.x + config::renderer::CHUNK_RENDER_DISTANCE; ++x)
        {
            for (int y = topLeft.y - config::renderer::CHUNK_RENDER_DISTANCE; y <= bottomRight.y + config::renderer::CHUNK_RENDER_DISTANCE; ++y)
            {
                chunkManager->loadChunk({x, y});
            }
        }
    }
}
