#include "render.h"

namespace
{
	template<typename T>
	std::enable_if_t<std::is_integral_v<T> || std::is_floating_point_v<T>, bool>
	in_range(T range_min, T range_max, T value)
	{
		return value >= range_min
			&& value <= range_max;
	}

	template<typename T>
	std::enable_if_t<std::is_integral_v<T> || std::is_floating_point_v<T>, T> 
	clamp(T range_min, T range_max, T value)
	{
		if(in_range(range_min, range_max, value))
		{
			return value;
		}
		else if(value < range_min)
		{
			return range_min;
		}
		else
		{
			return range_max;
		}
	}

	glm::vec4 extend_to_4(glm::vec3 const& v)
	{
		return {v.x, v.y, v.z, 1.0f};
	}

	glm::vec3 apply_transform(glm::vec3 const& point, glm::mat4x4 const& transform)
	{
		return glm::vec3(transform * extend_to_4(point));
	}
}

namespace render
{
	void wireframe(model const& object, glm::mat4x4 const& transform, image_utils::pixel const& color, image& target)
	{
		auto const width = target.width();
		auto const height = target.height();
		for(auto const& face : object)
		{
			for(size_t i = 0; i < face.size(); ++i)
			{
				auto const& first_vertex = apply_transform(face[i], transform);
				auto const& second_vertex = apply_transform(face[(i + 1) % face.size()], transform);
				auto const x0 = static_cast<size_t>((first_vertex.x + 1.0f) * width / 2.0f);
				auto const y0 = static_cast<size_t>((first_vertex.y + 1.0f) * height / 2.0f);
				auto const x1 = static_cast<size_t>((second_vertex.x + 1.0f) * width / 2.0f);
				auto const y1 = static_cast<size_t>((second_vertex.y + 1.0f) * height / 2.0f);
				
				auto const p1 = image::pos{clamp((size_t)0, width, x0), clamp((size_t)0, height, y0)};
				auto const p2 = image::pos{clamp((size_t)0, width, x1), clamp((size_t)0, height, y1)};
				target.line(color, p1, p2);
			}
		}
	}
}
