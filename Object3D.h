#pragma once

#include <glm/glm.hpp>

class Object3D
{
public:
	glm::mat4 transform;

	virtual void draw() = 0;

};

