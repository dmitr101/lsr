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
    template<typename T>
    T sign(T num)
    {
        static_assert(std::is_floating_point_v<T> || std::is_signed_v<T>, "Only works with signed numbers!");
        return num / abs(num);
    }
}

using namespace std;

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
    auto const h_range = static_cast<size_t>(abs(static_cast<int64_t>(from.first - to.first)));
    auto const v_range = static_cast<size_t>(abs(static_cast<int64_t>(from.second - to.second)));
    if(h_range > v_range)
    {
        auto const [left, right] = from.first < to.first ? pair{from , to} : pair{to, from};
        auto const ratio = static_cast<double>(v_range) / h_range * sign(static_cast<double>(left.second - right.second));
        for(auto x = left.first; x <= right.first; ++x)
        {
            auto const y = static_cast<size_t>(floorl(left.second + ratio * (x - left.first)));
            point(color, {x, y});
        }
    }
    else
    {
        auto const [up, bottom] = from.second < to.second ? pair{from , to} : pair{to, from};
        auto const ratio = static_cast<float>(h_range) / v_range * sign(static_cast<double>(up.first - bottom.first));
        for(auto y = up.second; y <= bottom.second; ++y)
        {
            auto const x = static_cast<size_t>(floorl(up.first + ratio * (y - up.second)));
            point(color, {x, y});
        }
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
