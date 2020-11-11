#include "Fire3D.h"

#define IMAGE_ROWS 4
#define IMAGE_COLS 8
//static const int IMAGE_COLS = 8;

static float vertices[4][3] = {
 { -.3f, 0.5f, 0.0f },
 { -.3f, 0.0f, 0.0f },
 { .3f, 0.5f, 0.0f },
 { .3f, 0.0f, 0.0f },
};

Fire3D::Fire3D()
{
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);
	glGenBuffers(2, vbo);

	// vertices
	glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(0);

	// no colors
	glDisableVertexAttribArray(1);

	// textures coords
	glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(textCoords), textCoords, GL_STATIC_DRAW);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(2);

	SDL_Surface* image = SDL_LoadBMP("fire8x4.bmp");
	if (image != NULL)
	{
		glGenTextures(1, &this->textureId);
		glBindTexture(GL_TEXTURE_2D, textureId);
		// set the texture wrapping/filtering options (on the currently bound texture object)
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, image->w, image->h, 0, GL_BGRA,
			GL_UNSIGNED_BYTE, image->pixels);
		// glGenerateMipmap(GL_TEXTURE_2D);
	}
	SDL_FreeSurface(image);

}

Fire3D::~Fire3D()
{
}

void Fire3D::setFrame(int frame)
{
	int col = frame % IMAGE_COLS;
	int row = frame / IMAGE_COLS;

	this->textureOffset.x = col * 1.0f / IMAGE_COLS;
	this->textureOffset.y = row * 1.0f / IMAGE_ROWS;

	//// bottom left
	//textCoords[1][0] = 1.0f / IMAGE_COLS * col;
	//textCoords[1][1] = 1.0f / IMAGE_ROWS * row;

	//// top left
	//textCoords[0][0] = textCoords[1][0];
	//textCoords[0][1] = textCoords[1][1] + 1.0f / IMAGE_ROWS;

	//// top right
	//textCoords[2][0] = textCoords[1][0] + 1.0f / IMAGE_COLS;
	//textCoords[2][1] = textCoords[0][1];

	//// bottom right
	//textCoords[3][0] = textCoords[2][0];
	//textCoords[3][1] = textCoords[1][1];

	//glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
	//glBufferData(GL_ARRAY_BUFFER, sizeof(textCoords), textCoords, GL_STATIC_DRAW);

}

void Fire3D::draw(SceneViewer* v)
{
//	v->setModelMatrix(this->transform); // already done by the scene!
	glBindVertexArray(vao);
	v->useTextures(1, &textureId, &this->textureOffset);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
}

