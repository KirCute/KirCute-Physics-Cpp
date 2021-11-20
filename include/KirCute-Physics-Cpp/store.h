#ifndef KIRCUTE_PHYSICS_CPP_STORE_H
#define KIRCUTE_PHYSICS_CPP_STORE_H

#include "rbody.h"

namespace kircute_physics {
    typedef std::function<Vec2f(const Rigidbody *const &rbody)> ForceField;

    class RigidbodyContainer {
    protected:
        Rect range;
        float validRange;

    public:
        explicit RigidbodyContainer(float validRange, const Rect &range = Rect(800, 600)) :
                validRange(validRange), range(range) {}

        virtual ~RigidbodyContainer() = default;

        virtual void push_back(Rigidbody *const &rbody) = 0;

        virtual void update(const std::vector<ForceField> &field) = 0;

        virtual void collideCheck() = 0;

        virtual void edgeCheck(float left, float top, float right, float bottom) = 0;

        virtual void render(Renderer *const &renderer) const = 0;

        Rect getRect() const { return range; }
    };
}

#endif //KIRCUTE_PHYSICS_CPP_STORE_H