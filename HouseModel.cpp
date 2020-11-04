#include "HouseModel.h"

#include <glad/glad.h>
#include <SDL.h>

static const GLfloat verticesColorsText[19][8] = {
{-2.5f, 2.5f, 3.0f, 1.0f, 1.0f, 1.0f, -2.5f, 2.5f}, // 0: Front, upper, left
{-2.5f, 0.0f, 3.0f, 1.0f, 1.0f, 1.0f, -2.5f, 0.0f}, // 1: lower, left
{ 0.0f, 3.0f, 3.0f, 1.0f, 1.0f, 1.0f, 0.0f, 3.0f}, // 2:
{ 2.5f, 0.0f, 3.0f, 0.0f, 1.0f, 1.0f, 2.5f, 0.0f}, // 3: lower, right
{ 2.5f, 2.5f, 3.0f, 0.0f, 1.0f, 1.0f, 2.5f, 2.5f}, // 4: upper, right
{ 2.5f, 0.0f, -3.0f, 1.0f, 1.0f, 1.0f, 8.5f, 0.0f}, // 5: Back, lower, right
{ 2.5f, 2.5f, -3.0f, 1.0f, 1.0f, 1.0f, 8.5f, 2.5f}, // 6: lower, right
{ 0.0f, 0.0f, -3.0f, 1.0f, 1.0f, 1.0f, 10.0f, 0.0f}, // 7: lower, left
{ 0.0f, 3.0f, -3.0f, 1.0f, 1.0f, 1.0f, 11.0f, 3.0f}, // 8:
{-2.5f, 0.0f, -3.0f, 1.0f, 1.0f, 1.0f, 13.5f, 0.0f}, // 9: lower, left
{-2.5f, 2.5f, -3.0f, 1.0f, 1.0f, 1.0f, 13.5f, 2.5f}, // 10: upper, left
{-2.5f, 0.0f, 3.0f, 1.0f, 1.0f, 1.0f, 19.5f, 0.0f,},// 11: Front, lower, left
{-2.5f, 2.5f, 3.0f, 0.0f, 1.0f, 1.0f, 19.5f, 2.5f}, // 12: upper, left
{ 2.5f, 2.5f, 3.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f}, // 13: Roof right
{ 2.5f, 2.5f, -3.0f, 1.0f, 0.0f, 0.0f, 6.0f, 0.0f}, // 14:
{ 0.0f, 3.0f, 3.0f, 1.0f, 0.0f, 0.0f, 0.0f, 2.55f}, // 15: Roof center
{ 0.0f, 3.0f, -3.0f, 1.0f, 0.0f, 0.0f, 6.0f, 2.55f}, // 16:
{-2.5f, 2.5f, 3.0f, 1.0f, 0.0f, 0.0f, 0.0f, 5.1f}, // 17: Roof right
{-2.5f, 2.5f, -3.0f, 1.0f, 0.0f, 0.0f, 6.0f, 5.1f}, // 18:
};


HouseModel::HouseModel() {

	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	glGenBuffers(1, vbo);

	// vertices AND colors AND texture coord DATA
	glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
	glBufferData(GL_ARRAY_BUFFER, 19 * 8 * sizeof(GLfloat), verticesColorsText, GL_STATIC_DRAW);

	// coord attrib
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8*sizeof(GLfloat), 0);
	glEnableVertexAttribArray(0);

	// colors attrib
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (void*)(3*sizeof(GLfloat)));
	glEnableVertexAttribArray(1);

	// texture coord attrib
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (void*)(6 * sizeof(GLfloat)));
	glEnableVertexAttribArray(2);

	glGenTextures(3, this->textures);

	SDL_Surface* image = SDL_LoadBMP("wallstone256.bmp");
	if (image != NULL)
	{
		glBindTexture(GL_TEXTURE_2D, textures[0]);
		// set the texture wrapping/filtering options (on the currently bound texture object)
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, image->w, image->h, 0, GL_BGR,
			GL_UNSIGNED_BYTE, image->pixels);
		// glGenerateMipmap(GL_TEXTURE_2D);
	}
	SDL_FreeSurface(image);
	image = SDL_LoadBMP("clay256.bmp");
	if (image != NULL)
	{
		glBindTexture(GL_TEXTURE_2D, textures[1]);
		// set the texture wrapping/filtering options (on the currently bound texture object)
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, image->w, image->h, 0, GL_BGR,
			GL_UNSIGNED_BYTE, image->pixels);
		// glGenerateMipmap(GL_TEXTURE_2D);
	}
	SDL_FreeSurface(image);

	image = SDL_LoadBMP("hedge256.bmp");
	if (image != NULL)
	{
		glBindTexture(GL_TEXTURE_2D, textures[2]);
		// set the texture wrapping/filtering options (on the currently bound texture object)
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, image->w, image->h, 0, GL_BGRA,
			GL_UNSIGNED_BYTE, image->pixels);
		// glGenerateMipmap(GL_TEXTURE_2D);
	}
	SDL_FreeSurface(image);

}

void HouseModel::draw(SceneViewer* viewer)
{
	glBindVertexArray(vao);

	GLuint t[] = { this->textures[0], this->textures[2] };
	viewer->useTextures(2, t);

	glDrawArrays(GL_TRIANGLE_STRIP, 0, 13);

	GLuint r[] = { this->textures[1] };
	viewer->useTextures(1, r);
	glDrawArrays(GL_TRIANGLE_STRIP, 13, 6);
}
