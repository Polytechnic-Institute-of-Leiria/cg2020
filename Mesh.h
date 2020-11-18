#pragma once
#include <glm/glm.hpp>
#include <glad/glad.h>

#include <vector>

#include "SceneViewer.h"

struct Vertex {
	glm::vec3 Position;
	glm::vec3 Color;
	glm::vec2 TexCoords;
};

class Mesh {
	// render data
	unsigned int VBO, EBO;
	GLuint VAO;
	std::vector<Vertex> vertices;
	std::vector<unsigned int> indices;
public:
	// constructor
	Mesh(std::vector<Vertex> vertices, std::vector<unsigned int>
		indices);
	// render the mesh
	void draw(SceneViewer* viewer);
};
