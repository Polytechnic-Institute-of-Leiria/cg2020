#include "Node3D.h"

glm::mat4 Node3D::getAccumulatedTransform()
{
    if (!this->parent) {
        return this->transform;
    }
    return this->parent->getAccumulatedTransform() * this->transform;
}

Node3D::Node3D(Node3D* parent)
{
    this->parent = parent;
    if (this->parent) {
        this->parent->children.push_back(this);
    }
    this->transform = glm::mat4(1.0f);
}

void Node3D::draw(SceneViewer* viewer, bool transparent)
{
    for (Node3D *n: this->children)
    {
        n->draw(viewer, transparent);
    }
    viewer->setModelMatrix(this->getAccumulatedTransform());
    for (Mesh m : this->meshes) {
        m.draw(viewer, transparent);
    }
}

Node3D* Node3D::getNodeByName(std::string name)
{
    if (this->name == name) {
        return this;
    }
    for (Node3D* c : this->children) {
        Node3D* result = c->getNodeByName(name);
        if (result) {
            return result;
        }
    }
    return nullptr;
}
