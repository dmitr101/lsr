#include <array>
#include <vector>
#include <string_view>

#include <glm/glm.hpp>
#include <tiny_obj_loader.h>

class model;

namespace model_utils
{
    model load(std::string_view file_name) noexcept;
}

//Simple class representing polygonal meshes with 3 vertices per face
class model final
{
public:
    static constexpr size_t face_vertix_number = 3;
    using face = std::array<glm::vec3, face_vertix_number>;

public:
    model(std::vector<glm::vec3>&& vertices, std::vector<size_t>&& indices) noexcept;
    model(model&&) noexcept = default;
    model(model const&) noexcept = default;

    std::vector<glm::vec3> const& vertices() const { return vertices_; }
    std::vector<size_t> const& indices() const { return indices_; }

    size_t face_number() const { return indices_.size() / face_vertix_number; }
    face face_at(size_t ind) const;

    class face_iterator final
    {
        public:
            face_iterator(model const& parent, size_t index);
            face_iterator(face_iterator const& other);
            face_iterator& operator=(face_iterator const&) = delete;
            ~face_iterator() = default;

            model::face operator*() const;
            model::face operator->() const;
            face_iterator operator++();
            face_iterator operator++(int);

            bool operator==(face_iterator const& other) const;
            bool operator!=(face_iterator const& other) const;

        private:
            model const& parent_;
            size_t index_;
    };

    face_iterator faces_begin() const { return face_iterator(*this, 0); }
    face_iterator faces_end() const { return face_iterator(*this, face_number()); }

private:
    std::vector<glm::vec3> vertices_;
    std::vector<size_t> indices_;
};

namespace std
{
    model::face_iterator begin(model const& m)
    {
        return m.faces_begin();
    }

    model::face_iterator end(model const& m)
    {
        return m.faces_end();
    }
}