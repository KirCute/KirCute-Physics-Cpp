#include "world.h"
#include "quadtree_store.h"
#include <iostream>
#include <cfloat>

using namespace kircute_physics;

World::World(RigidbodyContainer *const &container) :
        objs(container) {}

World::~World() {
    std::cout << Rigidbody::Count << ' ' << NodeProxy::Count << ' ' << Node::Count << std::endl;
    delete objs;
    std::cout << Rigidbody::Count << ' ' << NodeProxy::Count << ' ' << Node::Count << std::endl;
}

void World::update() {
    objs->update(fields);
    if (enableCollide) objs->collideCheck();
    objs->edgeCheck(
            enableLeftEdge ? .0f : -FLT_MAX,
            enableTopEdge ? .0f : -FLT_MAX,
            enableRightEdge ? static_cast<float>(objs->getRect().width) : FLT_MAX,
            enableBottomEdge ? static_cast<float>(objs->getRect().height) : FLT_MAX
    );
}

void World::render(Renderer *const &renderer) const {
    objs->render(renderer);
}

void World::create(float mass, float radius, const Vec2f &pos, const Color &color) {
    objs->push_back(new Rigidbody(mass, radius, pos, color));
}

void World::addField(const ForceField &field) {
    fields.push_back(field);
}