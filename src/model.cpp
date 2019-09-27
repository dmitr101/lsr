#include "model.h"

#define TINYOBJLOADER_IMPLEMENTATION
#include <tiny_obj_loader.h>

namespace model_utils
{
    model load(std::string_view file_name) noexcept
    {
        return model{ {}, {} };
    }
}

model::model(std::vector<vertex>&& vertices, std::vector<size_t>&& indices) noexcept
    : vertices_(std::move(vertices))
    , indices_(std::move(indices)) {}

