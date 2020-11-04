#pragma once

#include <glad/glad.h>
#include "SceneViewer.h"

class TreeModel
{
	GLuint vao, vbo[4];
	GLuint texture;

public:
	TreeModel();
	void draw(SceneViewer* viewer);
};

