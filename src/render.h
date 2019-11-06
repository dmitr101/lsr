#pragma once
#include "image.h"
#include "model.h"

namespace render
{
namespace draw_2d
{
void clear(image_utils::pixel const &color, image &target);
void point(image_utils::pixel const &color, image::pos const &pos, image &target);
void line(image_utils::pixel const &color, image::pos const &from, image::pos const &to, image &target);
void filled_triangle(image_utils::pixel const &color, image::pos const &p1, image::pos const &p2, image::pos const &p3,
                     image &target);
void empty_triangle(image_utils::pixel const &color, image::pos const &p1, image::pos const &p2, image::pos const &p3,
                    image &target);
} // namespace draw_2d
namespace draw_3d
{
void wireframe(model const &object, glm::mat4x4 const &transform, image_utils::pixel const &color, image &target);
}
} // namespace render
