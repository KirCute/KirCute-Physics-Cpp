#include "vector_store.h"
#include <iostream>

using namespace kircute_physics;

VectorContainer::VectorContainer(float validRange, const Rect &range) : RigidbodyContainer(validRange, range) {}

VectorContainer::VectorContainer(float validRange) : RigidbodyContainer(validRange) {}

VectorContainer::~VectorContainer() {
    for (auto rbody : container) delete rbody;
    container.clear();
    std::cout << "Deleted Store." << std::endl;
}

void VectorContainer::push_back(Rigidbody *const &rbody) {
    container.push_back(rbody);
}

void VectorContainer::update(const std::vector<ForceField> &fields) {
    for (auto rbody : container) {
        for (const auto &field : fields) rbody->force += field(rbody);
        rbody->update();
    }
    auto itr = container.begin();
    while (itr != container.end()) {
        auto pos = (*itr)->getPosition();
        auto radius = (*itr)->getRadius();
        if (pos.x + radius < -validRange || pos.y + radius < -validRange ||
            pos.x - radius > (float) range.width + validRange || pos.y - radius > (float) range.height + validRange) {
            (*itr)->destroy();
            delete (*itr);
            itr = container.erase(itr);
        } else itr++;
    }
}

void VectorContainer::collideCheck() {
    for (unsigned int a = 0; a < container.size(); a++) {
        for (unsigned int b = a + 1; b < container.size(); b++) {
            Rigidbody::collideCheck(container[a], container[b]);
        }
    }
}

void VectorContainer::edgeCheck(float left, float top, float right, float bottom) {
    for (auto rbody : container) Rigidbody::edgeCheck(rbody, left, top, right, bottom);
}

void VectorContainer::render(Renderer *const &renderer) const {
    for (auto rbody : container) rbody->render(renderer);
}

void VectorContainer::clear() {
    for (auto rbody : container) delete rbody;
    container.clear();
}