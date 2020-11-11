// CG04.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <glad/glad.h>
#include <SDL.h>
#include <glm/glm.hpp>
#include <glm/ext.hpp>
#include <stdio.h>

#include "SceneViewer.h"
#include "Scene.h"
#include "Tree3D.h"
#include "House3D.h"
#include "Fire3D.h"

GLuint genTriangle() {
    const GLfloat vertices2D[3][3] = {
        { -0.5, -0.5, 0.0 }, /* Left point */
        { 0.5, -0.5, 0.0 }, /* Right point */
        { 0.0, 0.5, 0.0 } }; /* Top point */

    const GLfloat colors[3][3] = {
        { 1.0, 0.0, 0.0 }, /* Red */
        { 0.0, 1.0, 0.0 }, /* Green */
        { 1.0, 1.0, 1.0 } }; /* White */

    GLuint vao, vbo[2]; /* Create handles for our VAO and two VBOs */

    /* Allocate and assign a Vertex Array Object to our handle */
    glGenVertexArrays(1, &vao);
    /* Bind our Vertex Array Object as the current used object */
    glBindVertexArray(vao);

    /* Allocate and assign two Vertex Buffer Objects to our handle */
    glGenBuffers(2, vbo);
    /* Bind our first VBO as being the active buffer and storing vertex attributes
    (coordinates) */
    glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
    glBufferData(GL_ARRAY_BUFFER, 3*3* sizeof(GLfloat), vertices2D, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(0);

    /* Bind our second VBO as being the active buffer and storing vertex attributes
(colors) */
    glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
    glBufferData(GL_ARRAY_BUFFER, 9 * sizeof(GLfloat), colors, GL_STATIC_DRAW);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(1);

    return vao;
}

int main(int argc, char* argv[])
{
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        printf("Video Initialization Error: %s", SDL_GetError());
        return 1;
    }

    SceneViewer viewer(800, 600);
    Scene scene;

    Tree3D* tree1 = new Tree3D();
    tree1->transform = 
        glm::translate(tree1->transform, glm::vec3(-2.0f, 0.0f, -2.0f));
    scene.add(tree1);
    
    Tree3D *tree2 = new Tree3D();
    tree2->transform = 
        glm::translate(tree2->transform, glm::vec3(0.0f, 0.0f, -3.0f));
    scene.add(tree2);

    Tree3D *tree3 = new Tree3D();
    tree3->transform = 
        glm::translate(tree3->transform, glm::vec3(2.0f, 0.0f, -2.0f));
    scene.add(tree3);

    House3D* house = new House3D();
    house->transform = glm::translate(glm::mat4(1.0f), glm::vec3(-1, 0, -10));
    house->transform = glm::rotate(house->transform, glm::radians(30.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    scene.add(house);

    Fire3D* fire = new Fire3D();
    scene.add(fire);
    fire->transform = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, -1.0f));

    SDL_Event windowEvent;
    int frame = 0;
    while (true)
    {
        if (SDL_PollEvent(&windowEvent))
        {
            if (windowEvent.type == SDL_QUIT) {
                break;
            }
        }
        /* Game Loop */
        /* Make our background black */
        glClearColor(0.0, 0.0, 0.0, 1.0);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        
        fire->setFrame(frame);
        scene.draw(&viewer);

        viewer.swapBuffers();
        frame++;
    }

    SDL_Quit();

    return 0;
}


// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file