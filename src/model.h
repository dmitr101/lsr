#include <vector>
#include <string_view>

#include <glm/glm.hpp>
#include <tiny_obj_loader.h>

class model;

namespace model_utils
{
    model load(std::string_view file_name) noexcept;
}

class model
{
public:
    model(std::vector<glm::vec3>&& vertices, std::vector<size_t>&& indices) noexcept;
    model(model&&) noexcept = default;
    model(model const&) noexcept = default;

private:
    std::vector<glm::vec3> vertices_;
    std::vector<size_t> indices_;
};
