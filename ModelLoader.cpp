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

    // process all materials
    for (int matIndex = 0; matIndex < scene->mNumMaterials ; matIndex++)
    {
        processMaterial(scene->mMaterials[matIndex]);
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

    Material *material = this->materials[mesh->mMaterialIndex];
    
    for (unsigned int i = 0; i < mesh->mNumVertices; i++)
    {
        Vertex vertex;
        
        // Positions
        vertex.Position = glm::vec3(mesh->mVertices[i].x, mesh->mVertices[i].y, mesh->mVertices[i].z);
        // Colors
        for (int colorIndex = 0; colorIndex < AI_MAX_NUMBER_OF_COLOR_SETS; colorIndex++)
        {
            if (mesh->HasVertexColors(colorIndex)) {
                aiColor4D *color = mesh->mColors[colorIndex];
                vertex.Color = glm::vec4(color->r, color->g, color->b, color->a);
                break;
            }
        }
        // Normals
        if (mesh->HasNormals()) {
            vertex.Normal = glm::vec3(mesh->mNormals[i].x, mesh->mNormals[i].y, mesh->mNormals[i].z);
        }
        // Texture coordinates
        for (int texCoordIndex = 0; texCoordIndex < AI_MAX_NUMBER_OF_TEXTURECOORDS; texCoordIndex++)
        {
            if (mesh->mTextureCoords[texCoordIndex])
            {
                vertex.TexCoords = glm::vec2(mesh->mTextureCoords[texCoordIndex][i].x, mesh->mTextureCoords[texCoordIndex][i].y);
                break;
            }
        }
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
    return Mesh(vertices, indices, material);
}

void ModelLoader::processMaterial(aiMaterial* mat) {
    static const aiTextureType types[] = {
    aiTextureType_NONE,
    aiTextureType_DIFFUSE,
    aiTextureType_SPECULAR,
    aiTextureType_AMBIENT,
    aiTextureType_EMISSIVE,
    aiTextureType_HEIGHT,
    aiTextureType_NORMALS,
    aiTextureType_SHININESS,
    aiTextureType_OPACITY,
    aiTextureType_DISPLACEMENT,
    aiTextureType_LIGHTMAP,
    aiTextureType_REFLECTION,
    aiTextureType_BASE_COLOR,
    aiTextureType_NORMAL_CAMERA,
    aiTextureType_EMISSION_COLOR,
    aiTextureType_METALNESS,
    aiTextureType_DIFFUSE_ROUGHNESS,
    aiTextureType_AMBIENT_OCCLUSION,
    aiTextureType_UNKNOWN
    };

    const int typesCount = sizeof(types) / sizeof(types[0]);
    // Only one texture per type
    std::string* files[typesCount];
    for (int s = 0; s < typesCount; s++) {
        files[s] = nullptr;
    }
    for (unsigned int type = 0; type < typesCount; ++type) {
        aiString name;
        for (int j = 0; j < mat->GetTextureCount(types[type]); j++) {
            mat->GetTexture(types[type], j, &name);
            if (name.length > 0) {
                files[type] = new std::string(name.C_Str());
                printf("\t%i\t%i\t\'%s\'\n", j, types[type], name.C_Str());
                break;
            }
        }
    }
    glm::vec3* diffuseColor = nullptr;
    aiColor3D color;
    if (AI_SUCCESS == mat->Get(AI_MATKEY_COLOR_DIFFUSE, color)) {
        diffuseColor = new glm::vec3(color.r, color.g, color.b);
    }

    float shininess = 0;
    aiGetMaterialFloat(mat, AI_MATKEY_SHININESS, &shininess);

    Material* m = new Material(std::string(mat->GetName().C_Str()), diffuseColor, shininess,
        files[aiTextureType_DIFFUSE], files[aiTextureType_NORMALS], files[aiTextureType_OPACITY]);
    if (m) {
        this->materials.push_back(m);
    }
}


