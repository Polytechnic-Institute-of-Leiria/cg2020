#include "HouseModel.h"

#include <glad/glad.h>

static const GLfloat verticesAndColors[14][6] = {
{-2.5f, 0.0f, 3.0f, 1.0f, 1.0f, 1.0f}, // 0: Front, lower, left white corner
{ 2.5f, 0.0f, 3.0f, 1.0f, 1.0f, 1.0f}, // 1: lower, right
{ 2.5f, 2.5f, 3.0f, 0.0f, 1.0f, 1.0f}, // 2: upper, right
{-2.5f, 2.5f, 3.0f, 0.0f, 1.0f, 1.0f}, // 3: upper, left
{-2.5f, 0.0f, -3.0f, 1.0f, 1.0f, 1.0f}, // 4: Back, lower, left white corner
{ 2.5f, 0.0f, -3.0f, 1.0f, 1.0f, 1.0f}, // 5: lower, right
{ 2.5f, 2.5f, -3.0f, 0.0f, 1.0f, 1.0f}, // 6: upper, right
{-2.5f, 2.5f, -3.0f, 0.0f, 1.0f, 1.0f}, // 7: upper, left
{-2.7f, 2.5f, 3.2f, 1.0f, 0.0f, 0.0f}, // 8: Roof front, bottom, left red corner
{ 2.7f, 2.5f, 3.2f, 1.0f, 0.0f, 0.0f}, // 9:
{ 0.0f, 3.0f, 3.2f, 1.0f, 0.0f, 0.0f}, // 10:
{-2.7f, 2.5f, -3.2f, 1.0f, 0.0f, 0.0f}, // 11: Roof back, bottom, left red corner
{ 2.7f, 2.5f, -3.2f, 1.0f, 0.0f, 0.0f}, // 12:
{ 0.0f, 3.0f, -3.2f, 1.0f, 0.0f, 0.0f}, // 13:
};

static int indices[] = {
	0, 3, 1, 2, 5, 6, 4, 7, 0, 3, // Walls
	8, 10, 9, 13, 12, 11, 10, 8 // Roof
};

HouseModel::HouseModel() {

	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	glGenBuffers(2, vbo);

	// vertices AND colors DATA
	glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
	glBufferData(GL_ARRAY_BUFFER, 14 * 6 * sizeof(GLfloat), verticesAndColors, GL_STATIC_DRAW);

	// vertex attrib
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6*sizeof(GLfloat), 0);
	glEnableVertexAttribArray(0);
	// colors attrib
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (void*)(3*sizeof(GLfloat)));
	glEnableVertexAttribArray(1);

	// indices
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vbo[1]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

}

void HouseModel::draw()
{
	glBindVertexArray(vao);
	glDrawElements(GL_TRIANGLE_STRIP, 10, GL_UNSIGNED_INT, 0);
	glDrawElements(GL_TRIANGLE_STRIP, 8, GL_UNSIGNED_INT, (void*)(10 * sizeof(GLuint)));
}
