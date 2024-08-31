#ifndef _EARTHGL_MESH_
#define _EARTHGL_MESH_

class Mesh
{
public:
    Mesh(const std::vector<Vertex>& vertices, const std::vector<glm::uvec3>& indices);

    void Draw(const GLuint& shader);

private:
    std::vector<Vertex> mVertices;
    std::vector<glm::uvec3> mIndices;
    GLuint mVAO, mVBO, mEBO;
};

#endif
