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

        log(viewBounds.x_bounds.min, " ", viewBounds.x_bounds.max, " ", viewBounds.y_bounds.min, " ", viewBounds.y_bounds.max, " | ");
        log(topLeft.x, " ", topLeft.y, " ", bottomRight.x, " ", bottomRight.y);

        log(topLeft.x - config::renderer::CHUNK_RENDER_DISTANCE, " ", topLeft.y - config::renderer::CHUNK_RENDER_DISTANCE, " ", bottomRight.x + config::renderer::CHUNK_RENDER_DISTANCE, " ", bottomRight.y + config::renderer::CHUNK_RENDER_DISTANCE);

        log("\n");


        for (int x = topLeft.x - config::renderer::CHUNK_RENDER_DISTANCE; x <= bottomRight.x + config::renderer::CHUNK_RENDER_DISTANCE; ++x)
        {
            for (int y = topLeft.y - config::renderer::CHUNK_RENDER_DISTANCE; y <= bottomRight.y + config::renderer::CHUNK_RENDER_DISTANCE; ++y)
            {
                m_chunkManager->loadChunk({x, y});
            }
        }
    }
}
