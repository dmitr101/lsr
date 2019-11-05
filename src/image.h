#pragma once
#include <cstdint>
#include <cassert>
#include <tuple>
#include <vector>
#include <numeric>
#include <string_view>

class image;

namespace image_utils
{
    using pixel_component = uint8_t;
    using pixel = std::tuple<pixel_component, pixel_component, pixel_component>;
    static constexpr pixel_component PIXEL_MAX = std::numeric_limits<pixel_component>::max();

    namespace common_colors
    {
        static constexpr pixel BLACK = { 0, 0, 0 };
        static constexpr pixel WHITE = { PIXEL_MAX, PIXEL_MAX, PIXEL_MAX };
        static constexpr pixel RED = { 0 , 0, PIXEL_MAX };
        static constexpr pixel GREEN = { 0, PIXEL_MAX, 0 };
        static constexpr pixel BLUE = { PIXEL_MAX, 0, 0 };
    }

    // Save image in png format. Function will check for file format extension.
    bool save_to_png(image const& img, std::string_view file_name) noexcept;
}

// Simple class representing 3 components per pixel 8 bit ber component images. 
// Not optimized at all. Consider replacing with boost::multi_array.
class image
{
public:
    using pixel_component = image_utils::pixel_component;
    using pixel = image_utils::pixel;
    using data_container = std::vector<pixel>;
    using data_iterator = data_container::iterator;
    using pos = std::pair<size_t, size_t>;

public:
    image(size_t const width, size_t height) noexcept;
    image(image&&) noexcept = default;
    image(image const&) noexcept = default;

    size_t width() const { return width_; }
    size_t height() const { return height_; }

    pixel& at(pos const& p);
    pixel const& at(pos const& p) const;
    
    data_container& data() noexcept;
    data_container const& data() const noexcept;
    pixel_component const* data_raw() const noexcept;

private:
    size_t width_ = 0;
    size_t height_ = 0;
    data_container data_;
};

