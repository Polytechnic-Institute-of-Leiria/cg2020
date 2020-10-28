#include "House3D.h"

#include "HouseModel.h"

static HouseModel* houseModel = nullptr;
static int houseModelCount = 0;

House3D::House3D() {
	if (houseModel == nullptr) {
		houseModel = new HouseModel();
	}
	houseModelCount++;
	this->transform = glm::mat4(1.0f);
}

House3D::~House3D() {
	houseModelCount--;
	if (houseModelCount == 0) {
		delete(houseModel);
		houseModel = nullptr;
	}
}

void House3D::draw() {
	houseModel->draw();
}