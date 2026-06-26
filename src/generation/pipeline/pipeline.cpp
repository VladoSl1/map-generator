#include "pipeline.hpp"



namespace generation
{
    void GenerationPipeline::generate()
    {
        math::RngEngine rngEngine(m_worldSeed);
        math::UnifIntDistribution heightInterval(config::generation::INSIDE_PADDING,
                                                 m_height - config::generation::INSIDE_PADDING);
        math::UnifIntDistribution widthInterval(config::generation::INSIDE_PADDING,
                                                m_width - config::generation::INSIDE_PADDING);

        pipeline::samplePoints(&points, rngEngine, widthInterval, heightInterval, config::generation::NUM_POINTS);
    }
}
