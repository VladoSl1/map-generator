#include "pipeline.hpp"

#include "core/utils/random.hpp"
#include "generation/pipeline/delaunay_triangulation.hpp"
#include "generation/pipeline/point_sampling.hpp"

#include "core/config.hpp"


namespace generation
{
    void GenerationPipeline::generate()
    {
        math::RngEngine rngEngine(m_worldSeed);
        math::UnifIntDistribution heightInterval(config::generation::INSIDE_PADDING,
                                                 m_height - config::generation::INSIDE_PADDING);
        math::UnifIntDistribution widthInterval(config::generation::INSIDE_PADDING,
                                                m_width - config::generation::INSIDE_PADDING);

        points = pipeline::samplePoints(rngEngine, widthInterval, heightInterval, config::generation::NUM_POINTS);

        triangles = pipeline::triangulate(points);



    }
}
