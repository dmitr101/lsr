#include "model.h"

#include <assimp/Importer.hpp>

namespace model_utils
{
    model load(std::string_view file_name) noexcept
    {
        Assimp::Importer importer;
        return model{ {}, {} };
    }
}


model::model(std::vector<vertex>&& vertices, std::vector<size_t>&& indices)
    : vertices_(std::move(vertices))
    , indices_(std::move(indices)) {}

