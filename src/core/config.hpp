#pragma once


namespace config
{
    namespace window
    {
        inline constexpr int WINDOW_HEIGHT = 1000;
        inline constexpr int WINDOW_WIDTH = 1000;
        inline constexpr const char* TITLE = "Realistic 2D Map Generator";
        inline constexpr const int FPS = 24;
    }

    namespace generation
    {
        inline constexpr int NUM_POINTS = 100;
        inline constexpr int INSIDE_PADDING = 15;
    }

    namespace renderer
    {
        inline constexpr int POINT_RADIUS = 4;
    }

    namespace math
    {
        inline constexpr double EPSILON = 1e-9;
    }
}

