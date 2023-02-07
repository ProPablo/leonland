#include "Camera.h"
#include "../application.h"
#include <glm/ext/matrix_clip_space.hpp>

glm::mat4 Camera::GetMat4()
{
    auto &app = Application::GetApp();
    //mutate bounds to monitor a zoom but maintain the aspect ratio gotten by the app
    //                               This syntax works i guess
    camRect.Bounds = (glm::vec2) app.Application::AspectRatio * size;
    
    auto camArr = camRect.GetBounds();
    //Very important that the clipping planes are properly adjusted or the 2d drawings that have a z position of 0 will not be drawn
    return glm::ortho(camArr[0], camArr[1], camArr[2], camArr[3], -1.0f, 1.0f);
}
