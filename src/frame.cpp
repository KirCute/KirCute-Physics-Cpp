#include "frame.h"
#include <iostream>

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
    std::cout << "Deleted Frame." << std::endl;
}

void Frame::start() {
    srand(time(NULL));
    std::thread fpsCounter([&]() {
        fpsCounterRunning = true;
        auto last = clock();
        while (fpsCounterRunning) {
            while (clock() - last < 1000L && fpsCounterRunning);
            if (fpsCounterRunning) {
                last += 1000L;
                last_fps = counting_fps;
                counting_fps = 0;
            }
        }
    });
    onInit();
    while (!shutdown) update();
    fpsCounterRunning = false;
    onExit();
    fpsCounter.join();
}

void Frame::update() {
    shutdown = shutdown || renderer->isClosed();
    if (clock() - lastUpdate < fpsTime) return;
    counting_fps++;
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