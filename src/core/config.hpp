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
        inline constexpr int SEED = 0;  // 0 for random seed, any other number for specific constant

        inline constexpr int NUM_POINTS = 3000;
        inline constexpr int CHUNK_WIDTH = 1000;
        inline constexpr int CHUNK_HEIGHT = 1000;
        inline constexpr int RELAXATION_ITERATIONS = 2;

        /* Size of one voroni cell in pixels is roughly
         * CHUNK_WIDTH / SQRT(NUM_POINTS) -> 3000 / sqrt(3000) \approx 18 pixels */

        inline constexpr float CONTINENTAL_SCALE = 2000.0f;
        inline constexpr float MOUNTAIN_SCALE = 150.0f;
        inline constexpr const char *ELEVATION_NODE_TREE_ENCODED = "KAAC@BER4JKQkNAAU@BIpHD9PwkG@BlkMEA65H4T4L4XqUPwQTAACAvwwQAABgFUQYuB6FvgkWCiQIw/UoPwkuAAE@BJDQAH@BC@AIEAJBw@ABZEED0KV78YZmZmPwQDmpkZPwsAAIA/HAMAACBCBBM@B/BA==";

        /* If moisture scale were the same as continental scale, moisture map would mirror continental map,
         * which would create uniform biomes across the map. */
        inline constexpr float MOISTURE_SCALE = CONTINENTAL_SCALE * 0.5f;
        inline constexpr int MOISTURE_OCTAVE_COUNT = 5;

        inline constexpr float TEMPERATURE_SCALE = CONTINENTAL_SCALE * 0.3f;
        inline constexpr int TEMPERATURE_OCTAVE_COUNT = 3;
    }

    namespace renderer
    {
        inline constexpr bool SHOW_CHUNK_GRID = false;
        inline constexpr bool SHOW_POLYGON_OUTLINES = false;
        inline constexpr bool DISCRETE_BIOME_COLORS = false;

        inline constexpr float POINT_RADIUS = 1;
        inline constexpr float DEFAULT_CAMERA_ZOOM = 1.0f;
        inline constexpr float CAMERA_ZOOM_STEP = 0.125f;
        inline constexpr math::Interval<float> CAMERA_ZOOM_RANGE{0.1f, 5.0f};
        inline constexpr math::Point2Df DEFAULT_CAMERA_POSITION{0.0f, 0.0f};
        inline constexpr int CHUNK_RENDER_DISTANCE = 0;

        inline constexpr float SNOW_LINE_ELEVATION    = 1.f;
        inline constexpr float SNOW_LINE_TEMP_STRENGTH = 0.3f;
    }
}

