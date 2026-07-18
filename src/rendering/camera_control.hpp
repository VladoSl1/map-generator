#pragma once

#include <raylib.h>

namespace renderer
{
    class CameraController
    {
    public:
        CameraController(int screenWidth, int screenHeight);

        void update();

        const Camera2D& getCamera() const;

    private:
        Camera2D camera;
    };
}
