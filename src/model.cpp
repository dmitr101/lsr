#include "model.h"

#include <algorithm>
#include <cassert>

#define TINYOBJLOADER_IMPLEMENTATION

namespace model_utils
{
    struct tinyobj_load_res
    {
        tinyobj::attrib_t attrib;
        std::vector<tinyobj::shape_t> shapes;
        std::vector<tinyobj::material_t> materials;
        std::string err;
    };

    tinyobj_load_res checked_basic_load(std::string_view file_name)
    {
        tinyobj_load_res load_res;
        tinyobj::LoadObj(&load_res.attrib, &load_res.shapes, &load_res.materials, &load_res.err, std::string(file_name).data());
        assert(load_res.shapes.size() == 1);
        assert(load_res.shapes.back().mesh.num_face_vertices[0] == 3); //Maybe find better way to assert only polygonal meshes
        assert(load_res.attrib.vertices.size() % 3 == 0);
        return load_res;
    }

    std::vector<glm::vec3> convert_vertices(tinyobj_load_res const& load_res)
    {
        std::vector<glm::vec3> converted_vertices;
        auto const& original_vertices = load_res.attrib.vertices;
        converted_vertices.reserve(original_vertices.size() / 3);
        for(size_t i = 0; i < original_vertices.size(); i += 3)
        {
            auto const x = original_vertices[i + 0];
            auto const y = original_vertices[i + 1];
            auto const z = original_vertices[i + 2];
            converted_vertices.emplace_back(x, y, z);
        }
        return converted_vertices;
    }

    std::vector<size_t> convert_indices(tinyobj_load_res const& load_res)
    {
        std::vector<size_t> converted_indices;
        auto const& original_indices = load_res.shapes.back().mesh.indices;
        converted_indices.reserve(original_indices.size());
        std::transform(std::begin(original_indices), std::end(original_indices),
            std::back_inserter(converted_indices),
            [](tinyobj::index_t const& ind)  { return ind.vertex_index; });
        return converted_indices;
    }

    model load(std::string_view file_name) noexcept
    {
        auto const load_res = checked_basic_load(file_name);
        auto converted_vertices = convert_vertices(load_res);
        auto converted_indices = convert_indices(load_res);
        return model{ std::move(converted_vertices), std::move(converted_indices) };
    }
}

model::model(std::vector<glm::vec3>&& vertices, std::vector<size_t>&& indices) noexcept
    : vertices_(std::move(vertices))
    , indices_(std::move(indices)) {}

