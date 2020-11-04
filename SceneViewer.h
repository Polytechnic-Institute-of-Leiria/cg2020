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
	GLint texturesCountID = -1;

	void genProgram();

public:
	SceneViewer(int w, int h);
	~SceneViewer();

	void setModelMatrix(glm::mat4 matrix);
	void useTextures(bool use);
	void swapBuffers();
};

