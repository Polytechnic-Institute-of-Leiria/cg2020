#pragma once
#include "Object3D.h"
#include <glm/glm.hpp>

class Fire3D : public Object3D
{

	GLuint vao;
	GLuint vbo[2];
	GLuint textureId;

	glm::vec2 textureOffset;

	float textCoords[4][2] = {
		{0.0f, 1.0f/4},
		{0.0f, 0.0f},
		{1.0f/8, 1.0f/4},
		{1.0f/8, 0.0f}
	};

public:
	Fire3D();
	~Fire3D();

	void setFrame(int frame);
	void draw(SceneViewer* v);
};

