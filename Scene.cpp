#include "Scene.h"

#include <glm/ext.hpp>
#include "SceneViewer.h"

void Scene::add(Object3D* obj)
{
	this->objects.push_back(obj);
}

void Scene::draw(SceneViewer* viewer)
{
	for (Object3D* o : this->objects)
	{
		viewer->setModelMatrix(o->transform);
		o->draw();
	}
}

Scene::~Scene() {
	for (Object3D* o : this->objects)
	{
		delete(o);
	}
}
