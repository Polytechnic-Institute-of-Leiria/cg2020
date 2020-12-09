#include "Material.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

Material::Material(std::string name, glm::vec3* diffuseColor, GLfloat specular, std::string
	* diffuseTextureFile, std::string* normalTextureFile, std::string* alphaTextureFile)
{
	this->name = name;
	this->diffuseColor = diffuseColor;
	this->specular = specular;
	this->diffuseTextureFile = diffuseTextureFile;
	this->normalTextureFile = normalTextureFile;
	this->alphaTextureFile = alphaTextureFile;

	this->diffuseTexture = loadTexture(this->diffuseTextureFile);
	this->normalTexture = loadTexture(this->normalTextureFile);
	this->alphaTexture = loadTexture(this->alphaTextureFile);
}

GLuint* Material::getDiffuseTextures()
{
	if (diffuseTexture > 0) {
		return &this->diffuseTexture;
	}
	else {
		return nullptr;
	}
}

GLuint Material::loadTexture(std::string *texturefile)
{
	if (texturefile == nullptr || texturefile->empty()) {
		return 0;
	}

	int width, height, nrChannels;
	unsigned char* data = stbi_load(texturefile->c_str(), &width, &height,
		&nrChannels, 0);
	if (!data)
	{
		printf("Failed to load texture %s", texturefile->c_str());
		return 0;
	}

	GLuint textureId = 0;
	glGenTextures(1, &textureId);
	glBindTexture(GL_TEXTURE_2D, textureId);
	// set the texture wrapping/filtering options (on the currently bound texture object)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	if (nrChannels == 3) {
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB,
			GL_UNSIGNED_BYTE, data);
	}
	else {
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA,
			GL_UNSIGNED_BYTE, data);
	}
	stbi_image_free(data);

	return textureId;
}
