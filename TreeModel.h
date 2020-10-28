#pragma once

#include <glad/glad.h>

class TreeModel
{
	GLuint vao, vbo[4];
	GLuint texture;

public:
	TreeModel();
	void draw();
};

