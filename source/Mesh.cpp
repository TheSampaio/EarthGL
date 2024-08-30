#include "PCH.hpp"
#include "Mesh.hpp"

Mesh::Mesh(const std::vector<Vertex>& vertices, const std::vector<GLuint>& indices)
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
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, mIndices.size() * sizeof(GLuint), mIndices.data(), GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), nullptr);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<void*>(static_cast<llong>(offsetof(Vertex, Vertex::texture))));
    glEnableVertexAttribArray(1);
}

void Mesh::Draw(const GLuint& shader)
{
    glUseProgram(shader);
    glBindVertexArray(mVAO);
    glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(mIndices.size()), GL_UNSIGNED_INT, nullptr);
}
