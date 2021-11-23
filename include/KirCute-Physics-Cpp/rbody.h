#ifndef KIRCUTE_PHYSICS_CPP_RBODY_H
#define KIRCUTE_PHYSICS_CPP_RBODY_H

#include "render.h"
#include <vector>
#include <map>

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
        bool _destroy{false};

        static bool collide(const Rigidbody *const &rbodyA, const Rigidbody *const &rbodyB);

        static void solve(Rigidbody *const &rbodyA, Rigidbody *const &rbodyB);

        static void rebound(Rigidbody *const &rbody, bool dir, float swift);

    public:
        Vec2f force;
        Color color;
        std::map<std::string, std::string> dictionary;
        std::vector<CollideTrigger> triggers;
        RigidbodyUpdate onRefresh = [&](Rigidbody *const &) {};
        RigidbodyRender onRender = [&](const Rigidbody *const &, Renderer *const &) {};
        RigidbodyUpdate onDestroy = [&](Rigidbody *const &) {};

        static bool DrawVelocity;
        static int Count;

        Rigidbody(float mass, float radius, const Vec2f &pos, const Color &color = 0x7F7F7F);

        ~Rigidbody();

        void render(Renderer *const &renderer) const;

        void update();

        void destroy();

        static void collideCheck(Rigidbody *const &rbodyA, Rigidbody * const&rbodyB);

        static void edgeCheck(Rigidbody *const &rbody, float left, float top, float right, float bottom);

        void appendVelocity(const Vec2f &vel) { velocity += vel; };

        float getMass() const { return mass; }

        float getRadius() const { return radius; }

        Vec2f getPosition() const { return position; }

        Vec2f getVelocity() const { return velocity; }

        bool isDestroyed() const { return _destroy; }
    };
}

#endif //KIRCUTE_PHYSICS_CPP_RBODY_H