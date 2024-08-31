#include "PCH.hpp"
#include "Texture.hpp"

#ifndef STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_IMPLEMENTATION
	#include <stb_image.h>
#endif

Texture::Texture(const std::string& filepath, GLenum format, GLenum slot, GLenum filter)
{
	// Flips image on load
    stbi_set_flip_vertically_on_load(true);

    // Loads a image from disk
    unsigned char* bytes = stbi_load(filepath.c_str(), &mSize[0], &mSize[1], &mColumn, 0);

    glGenTextures(1, &mId);

    // Binds a 2D texture
    glBindTexture(GL_TEXTURE_2D, mId);

    // Set-ups the minification and magnification filters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    // Set-ups the texture's wrap behavior
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    // Set-ups the texture data
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, mSize[0], mSize[1], 0, GL_RGB, GL_UNSIGNED_BYTE, bytes);

    // Generates a mipmap for the 2D texture
    glGenerateMipmap(GL_TEXTURE_2D);

    // Free the data loaded from disk
    stbi_image_free(bytes);

    // Unbinds the 2D texture to avoid bugs
    glBindTexture(GL_TEXTURE_2D, 0);
}

Texture::~Texture()
{
	glDeleteTextures(1, &mId);
}

void Texture::Bind()
{
	glActiveTexture(mSlot);
    glBindTexture(GL_TEXTURE_2D, mId);
}
