#ifndef KIRCUTE_PHYSICS_CPP_FRAME_H
#define KIRCUTE_PHYSICS_CPP_FRAME_H

#include "world.h"
#include <ctime>

namespace kircute_physics {
    class Frame {
    public:
        int counting_fps{0};
        int last_fps{0};
        bool fpsCounterRunning{false};

        explicit Frame(Renderer *const &renderer,
                       RigidbodyContainer *const &container,
                       float fps = 60.0f);

        virtual ~Frame();

        void start();

        int getFPS() const { return last_fps; }

    protected:
        bool shutdown = false;
        Renderer *renderer;
        World *world;

        virtual void onInit() {}

        virtual void onLastUpdate() {}

        virtual void onRefresh() {}

        virtual void onRender() {}

        virtual void onExit() {}

    private:
        clock_t fpsTime;
        clock_t lastUpdate;

        void update();

        void render();
    };
}

#endif //KIRCUTE_PHYSICS_CPP_FRAME_H