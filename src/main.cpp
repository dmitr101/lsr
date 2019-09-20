#include "image.h"
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

int main()
{
    fmt::print("Beginning render... \n");

    auto img = create_default_image();
    image_utils::save_to_png(img, "test.png");

    fmt::print("Render finished.\n");
    return 0;
}
