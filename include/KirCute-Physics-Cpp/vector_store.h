#ifndef KIRCUTE_PHYSICS_CPP_VECTOR_STORE_H
#define KIRCUTE_PHYSICS_CPP_VECTOR_STORE_H

#include "store.h"

namespace kircute_physics {
    class VectorContainer : public RigidbodyContainer {
    private:
        std::vector<Rigidbody *> container;

    public:
        VectorContainer(float validRange, const Rect &range);

        explicit VectorContainer(float validRange);

        ~VectorContainer() override;

        void push_back(Rigidbody *const &rbody) override;

        void update(const std::vector<ForceField> &field) override;

        void collideCheck() override;

        void edgeCheck(float left, float top, float right, float bottom) override;

        void render(Renderer *const &renderer) const override;

        void clear() override;
    };
}

#endif //KIRCUTE_PHYSICS_CPP_VECTOR_STORE_H