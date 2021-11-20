#include "frame.h"
#include "quadtree_store.h"
#include "easyx_renderer.h"

using namespace kircute_physics;

class Demo : public Frame {
public:
    explicit Demo() : Frame(new EasyxRenderer(), new QuadtreeEntrance(50.0f)) {}

    void onInit() override {
        Branch::DrawBranchEdge = true;
        world->addField([&](const Rigidbody *const &rbody) {
            return Vec2f(.0f, 0.5f);
        });
    }

    void onRefresh() override {
        renderer->mouseHandle([&](const MouseMsg &msg) {
            if (msg.l_button_down) {
                world->create(1.0f, 10.0f, Vec2f(msg.x, msg.y));
            }
        });
    }
};

int main() {
    auto frame = new Demo();
    frame->start();
    delete frame;
    return 0;
}
