#include "Mesh.h"
#include "SceneViewer.h"

Mesh::Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, Material *m)
{
	this->vertices = vertices;
	this->indices = indices;
	this->material = m;

	// create buffers/arrays
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);
	glBindVertexArray(VAO);
	// load data into vertex buffers
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices[0],
		GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0],
		GL_STATIC_DRAW);
	// set the vertex attribute pointers
	// vertex Positions
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
	// vertex Colors
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex,
		Color));
	// vertex texture coords
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex,
		TexCoords));
	// normals
	glEnableVertexAttribArray(3);
	glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex,
		Normal));
	glBindVertexArray(0);
}

void Mesh::draw(SceneViewer* viewer, bool transparent)
{
	if (transparent != (material != nullptr && material->getAlphaTexture() > 0)) {
		return;
	}

	if (this->material != nullptr) {
		viewer->setDiffuseColor(material->getDiffuseColor());
		viewer->useTextures(material->diffuseCount(), material->getDiffuseTextures());
		viewer->setNormalTexture(material->getNormalTexture());
		viewer->setAlphaTexture(material->getAlphaTexture());
		viewer->setSpecular(material->getSpecular());
	}
	else {
		viewer->useTextures(0, nullptr);
	}
	glBindVertexArray(VAO);
	glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
}
