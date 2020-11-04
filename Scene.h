#pragma once

#include <vector>
#include <glad/glad.h>

#include "Object3D.h"
#include"SceneViewer.h"

class Scene {
	std::vector<Object3D*> objects;
public:
	void add(Object3D* obj);
	void draw(SceneViewer* viewer);
	
	~Scene();

};

