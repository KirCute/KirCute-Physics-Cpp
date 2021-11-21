#ifndef KIRCUTE_PHYSICS_CPP_RBODY_H
#define KIRCUTE_PHYSICS_CPP_RBODY_H

#include "render.h"
#include <vector>

namespace kircute_physics {
    class Rigidbody;

    typedef std::function<bool(Rigidbody *const &rbody)> CollideTrigger;
    typedef std::function<void(Rigidbody *const &rbody)> RigidbodyUpdate;
    typedef std::function<void(const Rigidbody *const &rbody, Renderer *const &renderer)> RigidbodyRender;

    class Rigidbody {
    private:
        float mass;
        float radius;
        Vec2f position;
        Vec2f velocity;

        static bool collide(const Rigidbody *const &rbodyA, const Rigidbody *const &rbodyB);

        static void solve(Rigidbody *const &rbodyA, Rigidbody *const &rbodyB);

        static void rebound(Rigidbody *const &rbody, bool dir, float swift);

    public:
        bool destroy{false};
        Vec2f force;
        Color color;
        long long flag{0L};
        std::vector<CollideTrigger> triggers;
        RigidbodyUpdate onRefresh = [&](Rigidbody *const &rbody) {};
        RigidbodyRender onRender = [&](const Rigidbody *const &rbody, Renderer *const &render) {};

        static bool DrawVelocity;
        static int Count;

        Rigidbody(float mass, float radius, const Vec2f &pos, const Color &color = 0x7F7F7F);

        ~Rigidbody();

        void render(Renderer *const &renderer) const;

        void update();

        static void collideCheck(Rigidbody *const &rbodyA, Rigidbody * const&rbodyB);

        static void edgeCheck(Rigidbody *const &rbody, float left, float top, float right, float bottom);

        float getMass() const { return mass; }

        float getRadius() const { return radius; }

        Vec2f getPosition() const { return position; }

        Vec2f getVelocity() const { return velocity; }
    };
}

#endif //KIRCUTE_PHYSICS_CPP_RBODY_H