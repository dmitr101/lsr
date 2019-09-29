#include <vector>
#include <string_view>
#include <glm/glm.hpp>

class model;

namespace model_utils
{
    model load(std::string_view file_name) noexcept;
}

class model
{
public:
    using vertex = glm::vec3;

public:
    model(std::vector<vertex>&& vertices, std::vector<size_t>&& indices) noexcept;
    model(model&&) noexcept = default;
    model(model const&) noexcept = default;

private:
    std::vector<vertex> vertices_;
    std::vector<size_t> indices_;
};
