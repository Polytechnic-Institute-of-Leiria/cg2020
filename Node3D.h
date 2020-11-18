#pragma once

#include <vector>
#include <string>

#include "Mesh.h"

#include "Object3D.h"
class Node3D : public Object3D
{
	std::vector<Node3D*> children;
	Node3D* parent;
	glm::mat4 getAccumulatedTransform();

public:
	Node3D(Node3D* parent = nullptr);
	void draw(SceneViewer* v);
	std::vector<Mesh> meshes;
	std::string name;

	Node3D* getNodeByName(std::string name);

};

