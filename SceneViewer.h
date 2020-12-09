#pragma once

#include <glad/glad.h>
#include <SDL.h>
#include <glm/glm.hpp>

class SceneViewer
{
	int width = 0;
	int height = 0;

	SDL_Window* window = nullptr;
	SDL_GLContext context = nullptr;

	GLuint shaderProgram = -1;
	
	GLint modelMatrixID = -1;
	GLint viewMatrixID = -1;
	GLint projectionMatrixID = -1;

	glm::mat4 viewMatrix;
	glm::mat4 projectionMatrix;

	int usedTextures = 0;
	GLint usedTexturesID = -1;
	GLuint difuseTexturesIDs[2];
	GLint normalTextureID;
	GLint alphaTextureID;
	GLint textOffset = -1;
	GLint diffuseColorID = -1;
	GLint shininessID = -1;

	void genProgram();

public:
	SceneViewer(int w, int h);
	~SceneViewer();

	void rotateCamera(float angle, glm::vec3 axis);
	void translateCamera(glm::vec3 translation);

	void setModelMatrix(glm::mat4 matrix);
	void useTextures(int textureCounts, GLuint textures[], glm::vec2* textureOffset = nullptr);
	void setAlphaTexture(GLuint textureId);
	void setNormalTexture(GLuint textureId);

	void setDiffuseColor(glm::vec3 color);
	void swapBuffers();
};

