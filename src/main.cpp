#include "image.h"
#include "model.h"
#include "render.h"
#include <fmt/format.h>

image create_default_image()
{
    using namespace image_utils::common_colors;
    image i{ 64, 64 };
    auto const p1 = image::pos{ 0, 31 };
    auto const p2 = image::pos{ 63, 31 };
    auto const p3 = image::pos{ 31, 16 };
    i.clear(BLACK);
    i.line(RED, p1, p2);
    i.line(GREEN, p2, p3);
    i.line(BLUE, p3, p1);

    return std::move(i);
}

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
	target.clear(image_utils::common_colors::WHITE);
	render::render_wireframe(m, target);
	image_utils::save_to_png(target, "test.png");

    	fmt::print("Render finished. Saved to test.png .\n");
    	return 0;
}
