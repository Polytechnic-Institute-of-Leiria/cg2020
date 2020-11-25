#pragma once

#include <string>
#include <vector>
#include <assimp/scene.h>

#include "Node3D.h"
#include "Mesh.h"

class ModelLoader
{
	// processes a node in a recursive fashion
	void processNode(aiNode* node, const aiScene* scene, Node3D* parent);
	Mesh processMesh(aiMesh* mesh, const aiScene* scene);
	void processMaterial(aiMaterial* mat);

	std::vector<Material*> materials;

public:
	Node3D* root = nullptr;

	// constructor, expects a filepath to a 3D model.
	ModelLoader(std::string const& path);

};

