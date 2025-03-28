#include "pch.hpp"
#include "types.hpp"

Vertex::Vertex(glm::vec3 position, glm::vec3 normal, glm::vec2 texture)
{
    this->position = position;
    this->normal = normal;
    this->texture = texture;
}
