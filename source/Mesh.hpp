#ifndef _EARTHGL_MESH_
#define _EARTHGL_MESH_

class Mesh
{
public:
    Mesh(const std::vector<Vertex>& vertices, const std::vector<GLuint>& indices);

    void Draw(const GLuint& shader);

private:
    std::vector<Vertex> mVertices;
    std::vector<GLuint> mIndices;
    GLuint mVAO, mVBO, mEBO;
};

#endif
