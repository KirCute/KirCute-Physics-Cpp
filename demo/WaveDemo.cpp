#include "frame.h"
#include "vector_store.h"
#include "easyx_renderer.h"

using namespace kircute_physics;

class Demo : public Frame {
public:
    explicit Demo() : Frame(new EasyxRenderer(), new VectorContainer(50.0f)) {}

    void onInit() override {
        world->enableCollide = false;
        world->addField([&](const Rigidbody *const &rbody) {
            return Vec2f(.0f, (300.0f - rbody->getPosition().y) / 100.0f);
        });
    }

    void onRefresh() override {
        renderer->mouseHandle([&](const MouseMsg &msg) {
            if (msg.l_button) {
                world->create(1.0f, 2.0f, Vec2f(msg.x, msg.y), Color::FromHSV(msg.x / 800.0f * 360.0f, 1.0f, .75f));
            }
        });
    }

    void onRender() override {
        renderer->drawLine(0, world->getRect().height / 2, world->getRect().width, world->getRect().height / 2, 0xAFAFAF);
        renderer->drawLine(world->getRect().width / 2, 0, world->getRect().width / 2, world->getRect().height, 0xAFAFAF);
    }
};

int main() {
    auto frame = new Demo();
    frame->start();
    delete frame;
    return 0;
}
