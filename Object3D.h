#pragma once

#include <glm/glm.hpp>
#include "SceneViewer.h"

class Object3D
{
public:
	glm::mat4 transform;

	virtual void draw(SceneViewer *v) = 0;

};

