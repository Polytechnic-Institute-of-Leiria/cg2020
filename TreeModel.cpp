#include "TreeModel.h"
#include <SDL.h>

static const GLfloat vertices[13][3] = {
{ -0.3, 0.0, 0.0 }, // trunk
{ -0.3, 1.0, 0.0 },
{ 0.3, 0.0, 0.3 },
{ 0.3, 1.0, 0.3 },
{ 0.3, 0.0, -0.3 },
{ 0.3, 1.0, -0.3 },
{ -0.3, 0.0, 0.0 },
{ -0.3, 1.0, 0.0 },
{ 0.0, 4.0, 0.0}, // Canopy
{ -1.0, 1.0, 0.0},
{ 0.707, 1.0, 0.8},
{ 0.707, 1.0, -0.8},
{ -1.0, 1.0, 0.0},
};
static const GLfloat colors[13][3] = {
{ 0.82, 0.41, 0.12 }, /* trunk */
{ 0.82, 0.51, 0.22 },
{ 0.72, 0.31, 0.22 },
{ 0.7, 0.51, 0.12 },
{ 0.8, 0.31, 0.12 },
{0.7, 0.51, 0.12 },
{ 0.8, 0.31, 0.12 },
{ 0.9, 0.41, 0.12 },
{ 0.2, 0.99, 0.1 }, // Canopy
{ 0.1, 0.9, 0.2 },
{ 0.3, 0.85, 0.1 },
{ 0.2, 0.9, 0.1 },
{ 0.2, 0.9, 0.1 },
};

static const GLfloat texels[13][2] = {
{ 0.408, 1.0 }, // trunk
{ 0.408, 0.79 },
{ 0.472, 1.0 },
{ 0.472, 0.79 },
{ 0.536, 1.0 },
{ 0.536, 0.79 },
{ 0.6, 1.0 },
{ 0.6, 0.79 },
{ 0.5, 0.01}, // Canopy
{ 0.012, 0.51},
{ 0.318, 0.725},
{ 0.7227, 0.7575},
{ 0.986, 0.495},
};

static const GLuint indices[13] = {
1, 0, 3, 2, 5, 4, 7, 6, // trunk
8, 9, 10, 11, 12
};

TreeModel::TreeModel()
{
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);
	glGenBuffers(4, vbo);

	// vertices
	glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
	glBufferData(GL_ARRAY_BUFFER, 13*3*sizeof(GLfloat), vertices, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(0);

	// colors
	glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
	glBufferData(GL_ARRAY_BUFFER, 13 * 3 * sizeof(GLfloat), colors, GL_STATIC_DRAW);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(1);

	// Texture coords
	glBindBuffer(GL_ARRAY_BUFFER, vbo[2]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(texels), texels, GL_STATIC_DRAW);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(2);
	SDL_Surface* image = SDL_LoadBMP("pine-tree.bmp");
	if (image != NULL)
	{
		glGenTextures(1, &this->texture);
		glBindTexture(GL_TEXTURE_2D, texture);
		// set the texture wrapping/filtering options (on the currently bound texture object)
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, image->w, image->h, 0, GL_RGB,
			GL_UNSIGNED_BYTE, image->pixels);
		// glGenerateMipmap(GL_TEXTURE_2D);
	}
	//free(image);

	// indices
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vbo[3]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

}

void TreeModel::draw()
{
	glActiveTexture(GL_TEXTURE0); // activate the texture unit
	glBindTexture(GL_TEXTURE_2D, texture);
	glBindVertexArray(vao);
	glDrawElements(GL_TRIANGLE_STRIP, 8, GL_UNSIGNED_INT, 0);
	glDrawElements(GL_TRIANGLE_FAN, 5, GL_UNSIGNED_INT, (void*)(8*sizeof(GLuint)));
}
