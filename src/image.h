#pragma once
#include <cstdint>
#include <cassert>
#include <tuple>
#include <vector>
#include <numeric>

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
        static constexpr pixel RED = { PIXEL_MAX, 0, 0 };
        static constexpr pixel GREEN = { 0, PIXEL_MAX, 0 };
        static constexpr pixel BLUE = { 0, 0, PIXEL_MAX };
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

    class pixel_column_ref
    {
        friend image;
        pixel_column_ref(
            data_container& data, 
            size_t const width, 
            size_t const height, 
            size_t const column) noexcept;

    public:
        pixel& operator[](size_t y) noexcept;
        pixel const& operator[](size_t y) const noexcept;

    private:
        data_container& data_;
        size_t const column_ = 0;
        size_t const width_ = 0;
        size_t const height_ = 0;
    };

    size_t width() const { return width_; }
    size_t height() const { return height_; }
    pixel_column_ref operator[](size_t x) noexcept;

    void clear(pixel const& color) noexcept;
    void point(pixel const& color, pos const& pos) noexcept;
    void line(pixel const& color, pos const& from, pos const& to) noexcept;
    
    data_container const& data() const noexcept;
    pixel_component const* data_raw() const noexcept;

private:
    size_t width_ = 0;
    size_t height_ = 0;
    data_container data_;
};

