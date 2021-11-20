#include "frame.h"

using namespace kircute_physics;

Frame::Frame(Renderer *const &renderer, RigidbodyContainer *const &container, float fps) {
    this->renderer = renderer;
    renderer->initalizeWindow(container->getRect());
    fpsTime = static_cast<clock_t>(1000.f / fps);
    renderer->setBackground(0xFFFFFF);
    world = new World(container);
    lastUpdate = clock();
}

Frame::~Frame() {
    delete renderer;
    delete world;
}

void Frame::start() {
    onInit();
    while (!shutdown) update();
    onExit();
}

void Frame::update() {
    shutdown = shutdown || renderer->isClosed();
    if (clock() - lastUpdate < fpsTime) return;
    lastUpdate += fpsTime;
    onRefresh();
    world->update();
    onLastUpdate();
    render();
}

void Frame::render() {
    renderer->clear();
    world->render(renderer);
    onRender();
    renderer->update();
}