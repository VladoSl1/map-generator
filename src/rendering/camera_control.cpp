#include "camera_control.hpp"
#include <raymath.h>

#include "core/config.hpp"
#include "raylib_adapters.hpp"

namespace renderer
{
    CameraController::CameraController(int screenWidth, int screenHeight)
    {
        camera.target = toRaylib(config::renderer::DEFAULT_CAMERA_POSITION); // Where the camera is looking in world space
        // camera.offset = { screenWidth / 2.0f, screenHeight / 2.0f }; // Screen center
        camera.rotation = 0.0f;
        camera.zoom = config::renderer::DEFAULT_CAMERA_ZOOM;
    }

    void CameraController::update()
    {
        // Drag: with Right Mouse Button
        if (IsMouseButtonDown(MOUSE_BUTTON_RIGHT))
        {
            Vector2 delta = GetMouseDelta();

            // scale the delta by the inverse of the zoom to maintain 1:1 movement
            delta = Vector2Scale(delta, -1.0f / camera.zoom);
            camera.target = Vector2Add(camera.target, delta);
        }

        // Zoom: Mouse Wheel
        float wheel = GetMouseWheelMove();
        if (wheel != 0.0f)
        {
            // get the world position currently under the mouse
            Vector2 mouseWorldPos = GetScreenToWorld2D(GetMousePosition(), camera);

            // apply zoom step
            float zoomIncrement = config::renderer::CAMERA_ZOOM_STEP;
            camera.zoom += wheel * zoomIncrement;

            // clamp zoom to prevent flipping or zooming too far out/in
            camera.zoom = std::clamp(camera.zoom,
                                     config::renderer::CAMERA_ZOOM_RANGE.min,
                                     config::renderer::CAMERA_ZOOM_RANGE.max);

            // shift the offset to the current mouse position and target to the saved world position.
            camera.offset = GetMousePosition();
            camera.target = mouseWorldPos;
        }
    }

    const Camera2D& CameraController::getCamera() const
    {
        return camera;
    }
}
