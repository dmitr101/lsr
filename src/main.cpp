#include "image.h"
#include "model.h"
#include "render.h"
#include <fmt/format.h>
#include <glm/gtx/transform.hpp>

int main(int argc, char** argv)
{
	if(argc != 2)
	{
    	fmt::print("Wrong arguments! \n");
		return -1;
	}

	fmt::print("Loading model from: {}", argv[1]);
	model m = model_utils::load(argv[1]);	

    fmt::print("Beginning render... \n");

	image target{512, 512};
	render::draw_2d::clear(image_utils::common_colors::BLACK, target);
	auto const Z_AXIS = glm::vec3{0.0f, 0.0f, 1.0f};
	auto const SCALE = 2.0f;
	auto const SCALE_VEC = glm::vec3{SCALE, SCALE, SCALE};
	auto const transform = glm::scale(glm::rotate(glm::radians(180.0f), Z_AXIS), SCALE_VEC);
	render::draw_3d::wireframe(m, transform, image_utils::common_colors::WHITE, target);
	image_utils::save_to_png(target, "test.png");

    fmt::print("Render finished. Saved to test.png.\n");
    return 0;
}
