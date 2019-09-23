#include "image.h"

#include <algorithm>
#include <cmath>
#include <cstdint>

#define STB_IMAGE_WRITE_IMPLEMENTATION
#define STBI_MSC_SECURE_CRT
#include <stb_image_write.h>
namespace image_utils
{
    bool save_to_png(image const & img, std::string_view file_name) noexcept
    {
        assert(file_name.size() > 4 && "File must have *.png format.");
        static constexpr char file_ext[] = ".png";
        static constexpr auto ext_size = sizeof(file_ext) - 1;
        assert(file_name.compare(file_name.size() - ext_size, ext_size, file_ext) == 0 && "File must have *.png format.");

        return stbi_write_png(
            file_name.data(),
            static_cast<int>(img.width()),
            static_cast<int>(img.height()),
            static_cast<int>(std::tuple_size_v<image::pixel>),
            img.data_raw(),
            0) != 0;
    }
}

namespace
{
    template<size_t MAIN_DIR, size_t SECONDARY_DIR>
    void generic_line(image& target, image::pixel const& color, image::pos const& from, image::pos const& to)
    {
        auto const[start, end] = std::get<MAIN_DIR>(from) < std::get<MAIN_DIR>(to) ? std::pair{ from , to } : std::pair{ to, from };
        auto const main_diff = static_cast<int64_t>(std::get<MAIN_DIR>(end) - std::get<MAIN_DIR>(start));
        auto const secondary_diff = static_cast<int64_t>(std::get<SECONDARY_DIR>(end) - std::get<SECONDARY_DIR>(start));
        auto const dir = secondary_diff != 0 ? (secondary_diff / abs(secondary_diff)) : 0;
        auto const derr = main_diff != 0 ? abs(secondary_diff) : 0;

        auto secondary_index = std::get<SECONDARY_DIR>(start);
        auto err = int64_t{ 0 };
        for (auto main_index = std::get<MAIN_DIR>(start); main_index <= std::get<MAIN_DIR>(end); ++main_index)
        {
            auto const next_point = [&]
            {
                auto result = image::pos{};
                std::get<MAIN_DIR>(result) = main_index;
                std::get<SECONDARY_DIR>(result) = secondary_index;
                return result;
            }();
            target.point(color, next_point);
            err += derr;
            if(err * 2 >= abs(main_diff))
            {
                secondary_index += dir;
                err -= abs(main_diff);
            }
        }
    }
}

image::image(size_t const width, size_t height) noexcept
    : data_(width * height)
    , width_(width)
    , height_(height) {}

image::pixel& image::at(pos const& p)
{
    auto const[x, y] = p;
    assert(x < width_);
    assert(y < height_);
    return data_[width_ * y + x];
}

image::pixel const& image::at(pos const& p) const
{
    auto const[x, y] = p;
    assert(x < width_);
    assert(y < height_);
    return data_[width_ * y + x];
}

void image::clear(pixel const & color) noexcept
{
    fill(begin(data_), end(data_), color);
}

void image::point(pixel const & color, pos const & pos) noexcept
{
    at(pos) = color;
}

void image::line(pixel const & color, pos const & from, pos const & to) noexcept
{
    auto const horizontal_range = abs(static_cast<int64_t>(to.first - from.first));
    auto const vertical_range = abs(static_cast<int64_t>(to.second - from.second));
    if(horizontal_range > vertical_range)
    {
        generic_line<0, 1>(*this, color, from, to);
    }
    else
    {
        generic_line<1, 0>(*this, color, from, to);
    }
}

image::data_container const & image::data() const noexcept
{
    return data_;
}

image::pixel_component const * image::data_raw() const noexcept
{
    return data_.empty()
        ? nullptr
        : reinterpret_cast<pixel_component const*>(data_.data());
}
