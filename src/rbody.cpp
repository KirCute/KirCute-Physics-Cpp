#include "rbody.h"

using namespace kircute_physics;

int Rigidbody::Count = 0;
bool Rigidbody::DrawVelocity = false;

Rigidbody::Rigidbody(float mass, float radius, const Vec2f &pos, const Color &color) :
        mass(mass),
        radius(radius),
        position(pos),
        color(color),
        velocity(.0f) {
    Count++;
}

Rigidbody::~Rigidbody() {
    Count--;
}

void Rigidbody::render(Renderer *const &renderer) const {
    renderer->drawFillCircle((int) position.x, (int) position.y, (int) radius, color);
    if (DrawVelocity)
        renderer->drawLine((int) position.x, (int) position.y, (int) (position.x + velocity.x),
                           (int) (position.y + velocity.y), 0x00FF00);
    onRender(this, renderer);
}

void Rigidbody::update() {
    onRefresh(this);
    velocity += force;
    force.x = .0f, force.y = .0f;
    position += velocity;
}

void Rigidbody::destroy() {
    if (_destroy) return;
    onDestroy(this);
    _destroy = true;
}

bool Rigidbody::collide(const Rigidbody *const &rbodyA, const Rigidbody *const &rbodyB) {
    auto closedDistance = rbodyA->getRadius() + rbodyB->getRadius();
    auto distanceSqr = (rbodyA->getPosition() - rbodyB->getPosition()).magnitudeSqr();
    return distanceSqr <= closedDistance * closedDistance;
}

void Rigidbody::solve(Rigidbody *const &rbodyA, Rigidbody *const &rbodyB) {
    auto direction = rbodyA->position - rbodyB->position;
    auto distance = direction.magnitude();
    if (F_EQUALS_F(distance, .0f)) {
        rbodyB->position.x += static_cast<float>(rand() % 10) / 20.0f;
        rbodyB->position.x += static_cast<float>(rand() % 10) / 20.0f;
        solve(rbodyA, rbodyB);
        return;
    }
    Vec2f ovdirA, keepA, ovdirB, keepB;
    rbodyA->velocity.decompose_v(direction, ovdirA, keepA);
    rbodyB->velocity.decompose_v(direction, ovdirB, keepB);
    auto vdirA =
            (ovdirA * (rbodyA->mass - rbodyB->mass) + ovdirB * 2.0f * rbodyB->mass) / (rbodyA->mass + rbodyB->mass);
    auto vdirB =
            (ovdirB * (rbodyB->mass - rbodyA->mass) + ovdirA * 2.0f * rbodyA->mass) / (rbodyA->mass + rbodyB->mass);
    rbodyA->velocity = keepA + vdirA;
    rbodyB->velocity = keepB + vdirB;

    auto magnitude = (rbodyA->radius + rbodyB->radius - distance) / 4.0f + .5f;
    auto normalize = direction.normalize() * magnitude;
    rbodyA->position += normalize;
    rbodyB->position -= normalize;
}

void Rigidbody::collideCheck(Rigidbody *const &rbodyA, Rigidbody *const &rbodyB) {
    if (collide(rbodyA, rbodyB)) {
        auto _solve = true;
        for (const auto& trigger : rbodyA->triggers) _solve &= trigger(rbodyB);
        for (const auto& trigger : rbodyB->triggers) _solve &= trigger(rbodyA);
        if (_solve) solve(rbodyA, rbodyB);
    }
}

void Rigidbody::rebound(Rigidbody *const &rbody, bool dir, float swift) {
    if (dir) {
        rbody->position.y += swift;
        rbody->velocity.y = -rbody->velocity.y;
    } else {
        rbody->position.x += swift;
        rbody->velocity.x = -rbody->velocity.x;
    }
}

void Rigidbody::edgeCheck(Rigidbody *const &rbody, float left, float top, float right, float bottom) {
    auto l_dis = left + rbody->getRadius() - rbody->getPosition().x;
    if (l_dis > .0f) Rigidbody::rebound(rbody, false, l_dis);
    auto t_dis = top + rbody->getRadius() - rbody->getPosition().y;
    if (t_dis > .0f) Rigidbody::rebound(rbody, true, t_dis);
    auto r_dis = right - rbody->getRadius() - rbody->getPosition().x;
    if (r_dis < .0f) Rigidbody::rebound(rbody, false, r_dis);
    auto b_dis = bottom - rbody->getRadius() - rbody->getPosition().y;
    if (b_dis < .0f) Rigidbody::rebound(rbody, true, b_dis);
}