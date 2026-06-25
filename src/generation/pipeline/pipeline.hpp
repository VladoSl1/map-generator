#pragma once

#include "core/config.hpp"
#include "core/math/math.hpp"
#include "core/utils/random.hpp"
#include "generation/pipeline/point_sampling.hpp"


namespace generation
{
    class GenerationPipeline
    {
    public:
        GenerationPipeline(int width, int height, int worldSeed)
            : m_width(width), m_height(height), m_worldSeed(worldSeed)
        {

        }

        void generate()
        {
            math::RngEngine rngEngine(m_worldSeed);
            math::UnifIntDistribution heightInterval(0, m_height - config::generation::INSIDE_PADDING);
            math::UnifIntDistribution widthInterval(0, m_width - config::generation::INSIDE_PADDING);

            std::vector<math::Point2D> points;
            pipeline::samplePoints(&points, &rngEngine, widthInterval, heightInterval, config::generation::NUM_POINTS);
        }

    private:
        int m_width, m_height, m_worldSeed;

    };
}
