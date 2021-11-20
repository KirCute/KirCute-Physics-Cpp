#ifndef KIRCUTE_PHYSICS_CPP_RBODY_H
#define KIRCUTE_PHYSICS_CPP_RBODY_H

#include "render.h"
#include <vector>

namespace kircute_physics {
    class Rigidbody {
    private:
        float mass;
        float radius;
        Vec2f position;
        Vec2f velocity;

    public:
        static int Count;
        bool destroy{false};
        Vec2f force;
        Color color;

        static bool DrawVelocity;

        Rigidbody(float mass, float radius, const Vec2f &pos, const Color &color = 0x7F7F7F);

        ~Rigidbody();

        void render(Renderer *const &renderer) const;

        void update();

        static bool collide(const Rigidbody *const &rbodyA, const Rigidbody *const &rbodyB);

        static void solve(Rigidbody *const &rbodyA, Rigidbody *const &rbodyB);

        static void rebound(Rigidbody *const &rbody, bool dir, float swift);

        static void edgeCheck(Rigidbody *const &rbody, float left, float top, float right, float bottom);

        float getMass() const { return mass; }

        float getRadius() const { return radius; }

        Vec2f getPosition() const { return position; }

        Vec2f getVelocity() const { return velocity; }
    };
}

#endif //KIRCUTE_PHYSICS_CPP_RBODY_H