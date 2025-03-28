#ifndef _EARTHGL_TEXTURE_
#define _EARTHGL_TEXTURE_

class Texture
{
public:
    Texture(const std::string& filePath, GLenum format, GLenum slot, GLenum filter = GL_LINEAR);
    ~Texture();

    // Binds the texture
	void Bind() const;

	// Unbinds the texture
	void Unbind() { glBindTexture(GL_TEXTURE_2D, 0); }

	// Returns the texture's ID
	GLuint GetId() const { return mId; }

	// Returns the texture's size
	std::array<GLint, 2> GetSize() const { return mSize; }

private:
    // Attributes
	GLuint mId;
    GLenum mSlot;
    GLint mColumn;
	std::array<GLint, 2> mSize;
};

#endif
