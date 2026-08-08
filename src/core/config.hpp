#pragma once

#include "core/math/interval.hpp"
#include "core/math/point2d.hpp"

namespace config
{
    namespace window
    {
        inline constexpr int WINDOW_HEIGHT = 1300;
        inline constexpr int WINDOW_WIDTH = 1300;
        inline constexpr const char* TITLE = "Realistic 2D Map Generator";
        inline constexpr const int FPS = 24;
    }

    namespace generation
    {
        inline constexpr int NUM_POINTS = 4000;
        inline constexpr int INSIDE_PADDING = 15;
        inline constexpr int CHUNK_WIDTH = 1000;
        inline constexpr int CHUNK_HEIGHT = 1000;
        inline constexpr int RELAXATION_ITERATIONS = 2;

        /* Size of one voroni cell in pixels is roughly
         * CHUNK_WIDTH / SQRT(NUM_POINTS) -> 500 / 14.1 \approx 35 pixels */

        inline constexpr float CONTINENTAL_SCALE = 1000.0f;
        inline constexpr float EROSION_SCALE = 50.0f;
        inline constexpr float DETAIL_SCALE = 6.0f;
        inline constexpr int DETAIL_OCTAVE_COUNT = 5;

        /* If moisture scale were the same as continental scale, moisture map would mirror continental map,
         * which would create uniform biomes across the map. */
        inline constexpr float MOISTURE_SCALE = CONTINENTAL_SCALE * 0.5f;
        inline constexpr int MOISTURE_OCTAVE_COUNT = 5;

        inline constexpr float TEMPERATURE_SCALE = CONTINENTAL_SCALE * 0.3f;
        inline constexpr int TEMPERATURE_OCTAVE_COUNT = 3;
    }

    namespace renderer
    {
        inline constexpr float POINT_RADIUS = 1;
        inline constexpr float DEFAULT_CAMERA_ZOOM = 1.0f;
        inline constexpr float CAMERA_ZOOM_STEP = 0.125f;
        inline constexpr math::Interval<float> CAMERA_ZOOM_RANGE{0.1f, 5.0f};
        inline constexpr math::Point2Df DEFAULT_CAMERA_POSITION{0.0f, 0.0f};
        inline constexpr int CHUNK_RENDER_DISTANCE = 2;
    }
}

