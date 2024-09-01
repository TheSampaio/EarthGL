#include "PCH.hpp"
#include "Mesh.hpp"
#include "Renderer.hpp"

Mesh::Mesh(const std::vector<Vertex>& vertices, const std::vector<glm::uvec3>& indices)
{
    mVertices = vertices;
    mIndices = indices;

    // VAO
    glGenVertexArrays(1, &mVAO);
    glBindVertexArray(mVAO);

    // VBO
    glGenBuffers(1, &mVBO);
    glBindBuffer(GL_ARRAY_BUFFER, mVBO);
    glBufferData(GL_ARRAY_BUFFER, mVertices.size() * sizeof(Vertex), mVertices.data(), GL_STATIC_DRAW);

    // EBO
    glGenBuffers(1, &mEBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mEBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, mIndices.size() * 3 * sizeof(GLuint), mIndices.data(), GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), nullptr);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<void*>(static_cast<llong>(offsetof(Vertex, Vertex::texture))));
    glEnableVertexAttribArray(1);
}

void Mesh::Draw()
{
    glBindVertexArray(mVAO);
    Renderer::Draw(mIndices);
}
