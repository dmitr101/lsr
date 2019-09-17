#include "image.h"
#include <fmt/format.h>

image create_default_image()
{
    using namespace image_utils::common_colors;
    image i{ 64, 64 };
    i.clear(BLACK);
    i.point(RED, { 15, 15 });
    i.point(GREEN, { 47, 15 });
    i.point(BLUE, { 31, 47 });

    return std::move(i);
}

int main()
{
    fmt::print("Beginning render... \n");

    auto img = create_default_image();
    image_utils::save_to_png(img, "test.png");

    fmt::print("Render finished.\n");
    return 0;
}
