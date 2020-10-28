#include "Scene.h"

#include <glm/ext.hpp>

void Scene::add(Object3D* obj)
{
	this->objects.push_back(obj);
}

void Scene::draw(GLuint modelMatrixId)
{
	for (Object3D* o : this->objects)
	{
		glUniformMatrix4fv(modelMatrixId, 1, GL_FALSE, value_ptr(o->transform));
		o->draw();
	}
}

Scene::~Scene() {
	for (Object3D* o : this->objects)
	{
		delete(o);
	}
}
