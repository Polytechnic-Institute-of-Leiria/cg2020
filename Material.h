#pragma once

#include <string>
#include <glm/glm.hpp>
#include <glad/glad.h>

class Material {
	std::string name;
	glm::vec3* diffuseColor;
	std::string* diffuseTextureFile;
	GLuint diffuseTexture = 0;
	void loadTexture();
public:
	Material(std::string name, glm::vec3* diffuseColor, std::string
		* diffuseTextureFile);

	inline int diffuseCount() { return diffuseTexture ? 1 : 0; }
	GLuint* getDiffuseTextures();

	inline glm::vec3 getDiffuseColor() {
		return diffuseColor ? *diffuseColor : glm::vec3(0.0f);
	}
};


