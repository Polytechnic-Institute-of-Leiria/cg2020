#pragma once

#include <glad/glad.h>
#include <SDL.h>

class SceneViewer
{
	int width = 0;
	int height = 0;

	SDL_Window* window = nullptr;
	SDL_GLContext context = nullptr;

	GLuint shaderProgram = -1;
	

	void genProgram();

public:
	SceneViewer(int w, int h);
	~SceneViewer();

	GLint modelMatrixID = -1;
	GLint viewMatrixID = -1;
	GLint projectionMatrixID = -1;

	void swapBuffers();
};

