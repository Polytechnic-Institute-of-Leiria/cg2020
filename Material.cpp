#include "Material.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

Material::Material(std::string name, glm::vec3* diffuseColor, std::string* diffuseTextureFile)
{
	this->name = name;
	this->diffuseColor = diffuseColor;
	this->diffuseTextureFile = diffuseTextureFile;
	if (this->diffuseTextureFile != nullptr && !this->diffuseTextureFile->empty()) {
		this->loadTexture();
	}
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

void Material::loadTexture()
{
	int width, height, nrChannels;
	unsigned char* data = stbi_load(this->diffuseTextureFile->c_str(), &width, &height,
		&nrChannels, 0);
	if (!data)
	{
		printf("Failed to load texture");
		return;
	}

	glGenTextures(1, &this->diffuseTexture);
	glBindTexture(GL_TEXTURE_2D, diffuseTexture);
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
}
