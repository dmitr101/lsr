#pragma once
#include "image.h"
#include "model.h"

namespace render
{
	void wireframe(model const& object, image_utils::pixel const& color, image& target);
}
