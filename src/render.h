#pragma once
#include "image.h"
#include "model.h"

namespace render
{
	void wireframe(model const& object, glm::mat4x4 const& transform, image_utils::pixel const& color, image& target);
}
