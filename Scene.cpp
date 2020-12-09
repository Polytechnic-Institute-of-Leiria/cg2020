#include "Scene.h"

#include <glm/ext.hpp>
#include "SceneViewer.h"

void Scene::add(Object3D* obj)
{
	this->objects.push_back(obj);
}

void Scene::draw(SceneViewer* viewer)
{
	// draw opaque geometry (non transparent)
	for (Object3D* o : this->objects)
	{
		viewer->setModelMatrix(o->transform);
		o->draw(viewer);
	}
	// Draw transparent geometry
	for (Object3D* o : this->objects)
	{
		viewer->setModelMatrix(o->transform);
		o->draw(viewer, true);
	}
}

Scene::~Scene() {
	for (Object3D* o : this->objects)
	{
		delete(o);
	}
}
