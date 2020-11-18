#include "ModelLoader.h"
#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <iostream>

using namespace std;

void matAssimpToGLM(aiMatrix4x4 aM, glm::mat4 &gM) {
    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            gM[i][j] = aM[j][i];
        }
    }
}

ModelLoader::ModelLoader(std::string const& path)
{
    // read file via ASSIMP
    Assimp::Importer importer;
    const aiScene* scene = importer.ReadFile(path, aiProcess_Triangulate |
        aiProcess_GenSmoothNormals | aiProcess_FlipUVs | aiProcess_CalcTangentSpace);
    // check for errors
    if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) 
    {
        cout << "ERROR::ASSIMP:: " << importer.GetErrorString() << endl;
        return;
    }
        // process ASSIMP's root node recursively
    this->root = new Node3D();
    processNode(scene->mRootNode, scene, this->root);

}

void ModelLoader::processNode(aiNode* node, const aiScene* scene, Node3D* parent)
{
    Node3D* n = new Node3D(parent);
    n->name = std::string(node->mName.C_Str());
    matAssimpToGLM(node->mTransformation, n->transform);
    // process each mesh located at the current node
    for (unsigned int i = 0; i < node->mNumMeshes; i++)
    {
        aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
        n->meshes.push_back(processMesh(mesh, scene));
    }
    // recursively process each of the children nodes
    for (unsigned int i = 0; i < node->mNumChildren; i++)
    {
        processNode(node->mChildren[i], scene, n);
    }
}

Mesh ModelLoader::processMesh(aiMesh* mesh, const aiScene* scene)
{
    // data to fill
    vector<Vertex> vertices;
    vector<unsigned int> indices;
    aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];
    aiColor3D color;
    if (material != nullptr) {
        material->Get(AI_MATKEY_COLOR_DIFFUSE, color);
    }
    for (unsigned int i = 0; i < mesh->mNumVertices; i++)
    {
        Vertex vertex;
        glm::vec3 vector; // we declare a placeholder vector since assimp uses its own
        
        // positions
        vector.x = mesh->mVertices[i].x;
        vector.y = mesh->mVertices[i].y;
        vector.z = mesh->mVertices[i].z;
        vertex.Position = vector;
        // colors
        vector.x = color.r;
        vector.y = color.g;
        vector.z = color.b;
        vertex.Color = vector;
        // texture coordinates
        if (mesh->mTextureCoords[0])
        {
            glm::vec2 vec;
            vec.x = mesh->mTextureCoords[0][i].x;
            vec.y = mesh->mTextureCoords[0][i].y;
            vertex.TexCoords = vec;
        }
        else
            vertex.TexCoords = glm::vec2(0.0f, 0.0f);
        vertices.push_back(vertex);
    }
    // retrieve the corresponding vertex indices.
    for (unsigned int i = 0; i < mesh->mNumFaces; i++)
    {
        aiFace face = mesh->mFaces[i];
        // retrieve all indices of the face and store them in the indices vector
        for (unsigned int j = 0; j < face.mNumIndices; j++)
            indices.push_back(face.mIndices[j]);
    }
    // return a mesh object created from the extracted mesh data
    return Mesh(vertices, indices);

}

