#pragma once

#include <string>
#include <glm/glm.hpp>
#include <glad/glad.h>

class Material {
	std::string name;
	glm::vec3* diffuseColor;
	GLfloat shininess = 0;
	std::string* diffuseTextureFile;
	GLuint diffuseTexture = 0;
	std::string* normalTextureFile;
	GLuint normalTexture = 0;
	std::string* alphaTextureFile;
	GLuint alphaTexture = 0;

	GLuint loadTexture(std::string* texturefile);
public:
	Material(std::string name, glm::vec3* diffuseColor, GLfloat shininess, std::string
		* diffuseTextureFile, std::string* normalTextureFile, std::string* alphaTextureFile);

	inline int diffuseCount() { return diffuseTexture ? 1 : 0; }
	GLuint* getDiffuseTextures();

	inline glm::vec3 getDiffuseColor() {
		return diffuseColor ? *diffuseColor : glm::vec3(0.0f);
	}
	inline GLuint getNormalTexture() { return normalTexture; }
	inline GLuint getAlphaTexture() { return alphaTexture; }

};


