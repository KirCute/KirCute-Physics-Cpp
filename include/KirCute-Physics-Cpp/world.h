#ifndef KIRCUTE_PHYSICS_CPP_WORLD_H
#define KIRCUTE_PHYSICS_CPP_WORLD_H

#include "store.h"

namespace kircute_physics {
    class World {
    private:
        RigidbodyContainer *objs;
        std::vector<ForceField> fields;

    public:
        bool enableCollide{true};
        bool enableTopEdge{true};
        bool enableBottomEdge{true};
        bool enableLeftEdge{true};
        bool enableRightEdge{true};

        explicit World(RigidbodyContainer *const &container);

        ~World();

        void update();

        void render(Renderer *const &renderer) const;

        Rigidbody *create(float mass, float radius, const Vec2f &pos, const Color &color = 0x7F7F7F);

        void clear();

        void addField(const ForceField &field);

        void clearField();

        Rect getRect() const { return objs->getRect(); }

        float getValidRange() const { return objs->getValidRange(); }
    };
}

#endif //KIRCUTE_PHYSICS_CPP_WORLD_H