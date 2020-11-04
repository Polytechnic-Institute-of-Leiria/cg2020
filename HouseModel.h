#pragma once

#include <glad/glad.h>
#include "SceneViewer.h"

class HouseModel
{
	GLuint vao, vbo[1];
	GLuint textures[3];

public:
	HouseModel();
	void draw(SceneViewer *viewer);
};

