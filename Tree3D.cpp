#include "Tree3D.h"

#include "TreeModel.h"

static TreeModel* treeModel = nullptr;
static int treeCount = 0;

Tree3D::Tree3D() {
	// Check if model exists
	if (treeModel == nullptr) {
		// create the model
		treeModel = new TreeModel();
	}
	treeCount++;
	this->transform = glm::mat4(1.0f);
}

Tree3D::~Tree3D() {
	treeCount--;
	if (treeCount == 0) {
		delete(treeModel);
		treeModel = nullptr;
	}
}

void Tree3D::draw(SceneViewer *v) {
	treeModel->draw(v);
}
