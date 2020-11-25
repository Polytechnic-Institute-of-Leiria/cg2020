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

	GLint texturesCountID = -1;
	GLuint texturesID[2];
	GLint textOffsetID = -1;
	GLint diffuseColorID = -1;

	void genProgram();

public:
	SceneViewer(int w, int h);
	~SceneViewer();

	void rotateCamera(float angle, glm::vec3 axis);
	void translateCamera(glm::vec3 translation);

	void setModelMatrix(glm::mat4 matrix);
	void useTextures(int textureCounts, GLuint textures[], glm::vec2* textureOffset = nullptr);
	void setDiffuseColor(glm::vec3 color);
	void swapBuffers();
};

