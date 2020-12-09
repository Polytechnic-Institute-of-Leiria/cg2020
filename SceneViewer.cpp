#define _CRT_SECURE_NO_DEPRECATE
#include "SceneViewer.h"

#include <stdlib.h>   // For _MAX_PATH definition
#include <stdio.h>
#include <malloc.h>

#include <glm/glm.hpp>
#include <glm/ext.hpp>

#define TEXTURE_DIFFUSE1 0x01
#define TEXTURE_DIFFUSE2 0x02
#define TEXTURE_NORMAL 0x04
#define TEXTURE_ALPHA 0x08

// Utils
static char* filetobuf(char* file)
{
    FILE* fptr;
    long length;
    char* buf;
    fptr = fopen(file, "rb"); /* Open file for reading */
    if (!fptr) /* Return NULL on failure */
        return NULL;
    fseek(fptr, 0, SEEK_END); /* Seek to the end of the file */
    length = ftell(fptr); /* Find out how many bytes into the file we are */
    buf = (char*)malloc(length + 1); /* Allocate a buffer for the entire length of the file and a
    null terminator */
    fseek(fptr, 0, SEEK_SET); /* Go back to the beginning of the file */
    fread(buf, length, 1, fptr); /* Read the contents of the file in to the buffer */
    fclose(fptr); /* Close the file */
    buf[length] = 0; /* Null terminator */
    return buf; /* Return the buffer */
}

SceneViewer::SceneViewer(int w, int h)
{
	this->width = w;
	this->height = h;

    // Prepare for window and context creation
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);

    // Create the window
    window = SDL_CreateWindow("CG", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
        this->width, this->height, SDL_WINDOW_OPENGL);
    if (window == NULL) {
        printf("Window Creation Error:: %s", SDL_GetError());
        return;
    }

    // Init OpenGL by defining a context
    this->context = SDL_GL_CreateContext(window);
    // Initialize Glad
    if (!gladLoadGLLoader((GLADloadproc)SDL_GL_GetProcAddress)) {
        printf("Failed to initialize GLAD");
        SDL_Quit();
        return;
    }

    // Generate the Graphics Pipeline Program
    genProgram();

    // Model Matrix
    //glm::mat4 model = glm::mat4(1.0f);
    modelMatrixID = glGetUniformLocation(shaderProgram, "model");
    //glUniformMatrix4fv(modelMatrixID, 1, GL_FALSE, value_ptr(model)); // done by setModelMatrix

    // View Matrix
    viewMatrix = glm::mat4(1.0f);
    // translating the scene in the reverse direction of where we want to move
    // moving 5 units on Z
    viewMatrix = glm::translate(viewMatrix, glm::vec3(0.0f, -2.0f, -5.0f));
    // set the view matrix value
    viewMatrixID = glGetUniformLocation(shaderProgram, "view");
    glUniformMatrix4fv(viewMatrixID, 1, GL_FALSE, value_ptr(viewMatrix));
    // Projection Matrix
    projectionMatrix = glm::perspective(glm::radians(45.0f), 800.0f / 600.0f, 0.1f, 100.0f);
    projectionMatrixID = glGetUniformLocation(shaderProgram, "projection");
    glUniformMatrix4fv(projectionMatrixID, 1, GL_FALSE, value_ptr(projectionMatrix));

    // texture uniform
    this->usedTexturesID = glGetUniformLocation(shaderProgram, "usedTextures");
    this->difuseTexturesIDs[0] = glGetUniformLocation(shaderProgram,
        "diffuseTexture1");
    this->difuseTexturesIDs[1] = glGetUniformLocation(shaderProgram,
        "diffuseTexture2");
    this->normalTextureID = glGetUniformLocation(shaderProgram,
        "normalTexture");
    this->alphaTextureID = glGetUniformLocation(shaderProgram, "alphaTexture");
    this->textOffset = glGetUniformLocation(shaderProgram, "textOffset");
    this->diffuseColorID = glGetUniformLocation(shaderProgram, "diffuseColor");
    this->specularID = glGetUniformLocation(shaderProgram, "specular");

    this->ambientComponentID = glGetUniformLocation(shaderProgram, "ambientComponent");
    glUniform3fv(this->ambientComponentID, 1, value_ptr(glm::vec3(.2f, .2f, 0.2f))); // default ambient "light"

    this->lightPositionID = glGetUniformLocation(shaderProgram, "lightPosition");
    glUniform3fv(this->lightPositionID, 1, value_ptr(glm::vec3(1.0f, 3.0f, 0.0f)));
    this->lightColorID = glGetUniformLocation(shaderProgram, "lightColor");
    glUniform3fv(this->lightColorID, 1, value_ptr(glm::vec3(0.8f, 0.8f, .8f)));

    /* Enable Z depth testing so objects closest to the viewpoint are in front */
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);

    // Enable blending
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

SceneViewer::~SceneViewer()
{
    SDL_DestroyWindow(window);
}

void SceneViewer::rotateCamera(float angle, glm::vec3 axis)
{
    glm::mat4 r = glm::rotate(glm::mat4(1.0f), angle, axis);
    this->viewMatrix = r * this->viewMatrix;
    glUniformMatrix4fv(viewMatrixID, 1, GL_FALSE, value_ptr(viewMatrix));
}

void SceneViewer::translateCamera(glm::vec3 translation)
{
    glm::mat4 t = glm::translate(glm::mat4(1.0f), translation);
    this->viewMatrix = t * this->viewMatrix;
    glUniformMatrix4fv(viewMatrixID, 1, GL_FALSE, value_ptr(viewMatrix));
}

glm::vec3 SceneViewer::getCameraPosition()
{
    glm::vec4 p = glm::inverse(this->viewMatrix) * glm::vec4(0.0f, .0f, .0f, 1.0f);
    return glm::vec3(p.x, p.y, p.z);
}


void SceneViewer::setModelMatrix(glm::mat4 matrix)
{
    glUniformMatrix4fv(this->modelMatrixID, 1, GL_FALSE, value_ptr(matrix));
}

void SceneViewer::useTextures(int textureCounts, GLuint textures[], glm::vec2* textureOffset)
{
    if (textureOffset == nullptr) {
        glUniform2fv(textOffset, 1, value_ptr(glm::vec2(0.0f)));
    }
    else {
        glUniform2fv(textOffset, 1, value_ptr(*textureOffset));
    }

    if (textureCounts >= 1) {
        this->usedTextures = this->usedTextures | TEXTURE_DIFFUSE1;
        glActiveTexture(GL_TEXTURE0 + 3); // use Texture Unit 3 just because...
        glBindTexture(GL_TEXTURE_2D, textures[0]);
        glUniform1i(this->difuseTexturesIDs[0], 3);
        if (textureCounts >= 2) {
            this->usedTextures = this->usedTextures | TEXTURE_DIFFUSE2;
            glActiveTexture(GL_TEXTURE0 + 5);
            glBindTexture(GL_TEXTURE_2D, textures[1]);
            glUniform1i(this->difuseTexturesIDs[1], 5);
        }
    }
    else {
        this->usedTextures = this->usedTextures & ~TEXTURE_DIFFUSE1 & ~TEXTURE_DIFFUSE2;
    }
    glUniform1i(this->usedTexturesID, this->usedTextures);
}

void SceneViewer::setAlphaTexture(GLuint textureId) {
    if (textureId) {
        this->usedTextures |= TEXTURE_ALPHA;
        glActiveTexture(GL_TEXTURE0 + 7);
        glBindTexture(GL_TEXTURE_2D, textureId);
        glUniform1i(this->alphaTextureID, 7);
    }
    else {
        this->usedTextures &= ~TEXTURE_ALPHA;
    }
    glUniform1i(this->usedTexturesID, this->usedTextures);
}

void SceneViewer::setNormalTexture(GLuint textureId) {
    if (textureId) {
        this->usedTextures |= TEXTURE_NORMAL;
        glActiveTexture(GL_TEXTURE0 + 8);
        glBindTexture(GL_TEXTURE_2D, textureId);
        glUniform1i(this->normalTextureID, 8);
    }
    else {
        this->usedTextures &= ~TEXTURE_NORMAL;
    }
    glUniform1i(this->usedTexturesID, this->usedTextures);
}

void SceneViewer::setDiffuseColor(glm::vec3 color)
{
    glUniform3fv(this->diffuseColorID, 1, value_ptr(color));
}

void SceneViewer::setSpecular(GLfloat specular)
{
    glUniform1f(this->specularID, specular);
}

void SceneViewer::swapBuffers()
{
    /* Swap our buffers to make our changes visible */
    SDL_GL_SwapWindow(window);

    usedTextures = 0;
    glUniform1i(this->usedTexturesID, usedTextures);
}

void SceneViewer::setLightPosition(glm::vec3 position)
{
    glUniform3fv(this->lightPositionID, 1, value_ptr(position));
}

void SceneViewer::genProgram() {
    GLint IsCompiled_VS, maxLength, IsCompiled_FS, IsLinked;
    char* vertexInfoLog, * fragmentsource, * fragmentInfoLog, * shaderProgramInfoLog;

    ////////////////////
    // VERTEX SHADER

    /* Read our shaders into the appropriate buffers */
    char* vertexsource = filetobuf((char*)"cg03.vert");
    /* Create an empty vertex shader handle */
    GLuint vertexshader = glCreateShader(GL_VERTEX_SHADER);
    /* Send the vertex shader source code to GL */
    /* Note that the source code is NULL character terminated. */
    glShaderSource(vertexshader, 1, (const GLchar**)&vertexsource, 0);
    /* Compile the vertex shader */
    glCompileShader(vertexshader);
    glGetShaderiv(vertexshader, GL_COMPILE_STATUS, &IsCompiled_VS);
    if (IsCompiled_VS == false)
    {
        glGetShaderiv(vertexshader, GL_INFO_LOG_LENGTH, &maxLength);
        /* The maxLength includes the NULL character */
        vertexInfoLog = (char*)malloc(maxLength);
        glGetShaderInfoLog(vertexshader, maxLength, &maxLength, vertexInfoLog);
        /* Handle the error in an appropriate way */
        /* In this simple program, we'll just leave */
        free(vertexInfoLog);
        return;
    }

    ////////////////////
    // FRAGMENT SHADER

    /* Read our shaders into the appropriate buffers */
    fragmentsource = filetobuf((char*)"cg03.frag");
    /* Create an empty fragment shader handle */
    GLuint fragmentshader = glCreateShader(GL_FRAGMENT_SHADER);
    /* Send the fragment shader source code to GL */
    /* Note that the source code is NULL character terminated. */
    /* GL will automatically detect that therefore the length info can be 0 in this case (the
    last parameter) */
    glShaderSource(fragmentshader, 1, (const GLchar**)&fragmentsource, 0);
    /* Compile the fragment shader */
    glCompileShader(fragmentshader);
    glGetShaderiv(fragmentshader, GL_COMPILE_STATUS, &IsCompiled_FS);
    if (IsCompiled_FS == false)
    {
        glGetShaderiv(fragmentshader, GL_INFO_LOG_LENGTH, &maxLength);
        /* The maxLength includes the NULL character */
        fragmentInfoLog = (char*)malloc(maxLength);
        glGetShaderInfoLog(fragmentshader, maxLength, &maxLength, fragmentInfoLog);
        /* Handle the error in an appropriate way */
        /* In this simple program, we'll just leave */
        free(fragmentInfoLog);
        return;
    }


    ////////////////////
    // PROGRAM

    /* Assign our program handle a "name" */
    this->shaderProgram = glCreateProgram();

    /* Attach our shaders to our program */
    glAttachShader(shaderProgram, vertexshader);
    glAttachShader(shaderProgram, fragmentshader);

    /* Attribute locations must be setup before calling glLinkProgram. */
    glBindAttribLocation(shaderProgram, 0, "in_Position");
    glBindAttribLocation(shaderProgram, 1, "in_Color");
    glBindAttribLocation(shaderProgram, 2, "in_TexCoord");
    glBindAttribLocation(shaderProgram, 3, "in_Normal");

    glLinkProgram(shaderProgram);
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, (int*)&IsLinked);
    if (IsLinked == false)
    {
        /* Noticed that glGetProgramiv is used to get the length for a shader program, not
        glGetShaderiv. */
        glGetProgramiv(shaderProgram, GL_INFO_LOG_LENGTH, &maxLength);
        /* The maxLength includes the NULL character */
        shaderProgramInfoLog = (char*)malloc(maxLength);
        /* Notice that glGetProgramInfoLog, not glGetShaderInfoLog. */
        glGetProgramInfoLog(shaderProgram, maxLength, &maxLength,
            shaderProgramInfoLog);
        /* Handle the error in an appropriate way */
        /* In this simple program, we'll just leave */
        free(shaderProgramInfoLog);
        return;
    }
    /* Load the shader into the rendering pipeline */
    glUseProgram(shaderProgram);
}
