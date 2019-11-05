#include "render.h"

using namespace std;

namespace
{
	template<size_t MAIN_DIR, size_t SECONDARY_DIR>
    void generic_line(image& target, image::pixel const& color, image::pos const& from, image::pos const& to)
    {
        auto const[start, end] = get<MAIN_DIR>(from) < get<MAIN_DIR>(to) ? pair{ from , to } : pair{ to, from };
        auto const main_diff = static_cast<int64_t>(get<MAIN_DIR>(end) - get<MAIN_DIR>(start));
        auto const secondary_diff = static_cast<int64_t>(get<SECONDARY_DIR>(end) - get<SECONDARY_DIR>(start));
        auto const dir = secondary_diff != 0 ? (secondary_diff / abs(secondary_diff)) : 0;
        auto const derr = main_diff != 0 ? abs(secondary_diff) : 0;

        auto secondary_index = get<SECONDARY_DIR>(start);
        auto err = int64_t{ 0 };
        for (auto main_index = get<MAIN_DIR>(start); main_index <= get<MAIN_DIR>(end); ++main_index)
        {
            auto const next_point = [&]
            {
                auto result = image::pos{};
                get<MAIN_DIR>(result) = main_index;
                get<SECONDARY_DIR>(result) = secondary_index;
                return result;
            }();
            render::draw_2d::point(color, next_point, target);
            err += derr;
            if(err * 2 >= abs(main_diff))
            {
                secondary_index += dir;
                err -= abs(main_diff);
            }
        }
    }

	void triangle_line_sweeping(image_utils::pixel const& color, image::pos const& p1, image::pos const& p2, image::pos const& p3, image& target)
	{
		array points{p1, p2, p3};
		sort(begin(points), end(points), [](auto const& p1, auto const& p2){return p1.second < p2.second;});
		auto const total_height = points[2].second - points[0].second;
		for(size_t i = 0; i < total_height; ++i);
	}

	void triangle_entire_bbox(image_utils::pixel const& color, image::pos const& p1, image::pos const& p2, image::pos const& p3, image& target)
	{
	}

	glm::vec4 extend_to_4(glm::vec3 const& v)
	{
		return {v.x, v.y, v.z, 1.0f};
	}

	glm::vec3 apply_transform(glm::vec3 const& point, glm::mat4x4 const& transform)
	{
		return glm::vec3(transform * extend_to_4(point));
	}

	model::face apply_transform(model::face const& triangle, glm::mat4x4 const& transform)
	{
		model::face result;
		std::transform(begin(triangle), end(triangle), begin(result), 
			[&transform](auto const& v) 
			{
				return apply_transform(v, transform);
			});
		return result;
	}

	image::pos to_screen_coords(glm::vec3 const& v, size_t const width, size_t const height)
	{
		auto const x = static_cast<size_t>((v.x + 1.0f) * width / 2.0f);
		auto const y = static_cast<size_t>((v.y + 1.0f) * height / 2.0f);
		return { glm::clamp(x, 0ull, width), glm::clamp(y, 0ull, height) };
	}

	array<image::pos, 3> to_screen_coords(model::face const& triangle, size_t const width, size_t const height)
	{
		array<image::pos, 3> result;
		std::transform(begin(triangle), end(triangle), begin(result), 
			[width, height](auto const& v) 
			{ 
				return to_screen_coords(v, width, height); 
			});
		return result;
	} 
}

namespace render
{
	namespace draw_2d
	{
		void clear(image_utils::pixel const& color, image& target)
		{
    		fill(begin(target.data()), end(target.data()), color);
		}

    	void point(image_utils::pixel const& color, image::pos const& pos, image& target)
		{
    		target.at(pos) = color;
		}

    	void line(image_utils::pixel const& color, image::pos const& from, image::pos const& to, image& target)
		{
			auto const horizontal_range = abs(static_cast<int64_t>(to.first - from.first));
    		auto const vertical_range = abs(static_cast<int64_t>(to.second - from.second));
    		if(horizontal_range > vertical_range)
    		{
    		    generic_line<0, 1>(target, color, from, to);
    		}
    		else
    		{
    		    generic_line<1, 0>(target, color, from, to);
    		}
		}

		void filled_triangle(image_utils::pixel const& color, image::pos const& p1, image::pos const& p2, image::pos const& p3, image& target)
		{
			#if defined(RASTER_LINE_SWEEPING)
			triangle_line_sweeping(color, p1, p2, p3, target);
			#else
			triangle_entire_bbox(color, p1, p2, p3, target);
			#endif
		}

		void empty_triangle(image_utils::pixel const& color, image::pos const& p1, image::pos const& p2, image::pos const& p3, image& target)
		{
			line(color, p1, p2, target);
			line(color, p2, p3, target);
			line(color, p1, p3, target);
		}
	}

	namespace draw_3d
	{
		void wireframe(model const& object, glm::mat4x4 const& transform, image_utils::pixel const& color, image& target)
		{
			auto const width = target.width();
			auto const height = target.height();
			for(auto const& face : object)
			{
				auto const transformed_face = apply_transform(face, transform);
				auto const screen_face = to_screen_coords(transformed_face, width, height);
				draw_2d::empty_triangle(color, screen_face[0], screen_face[1], screen_face[2], target);
			}
		}
	}
}
