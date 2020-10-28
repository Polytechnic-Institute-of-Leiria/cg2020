#pragma once

#include <glad/glad.h>

class HouseModel
{
	GLuint vao, vbo[2];

public:
	HouseModel();
	void draw();
};

