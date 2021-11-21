#ifndef KIRCUTE_PHYSICS_CPP_EASYX_H
#define KIRCUTE_PHYSICS_CPP_EASYX_H

#include "render.h"
#include <graphics.h>

namespace kircute_physics {
    class EasyxRenderer : public Renderer {
    private:
        MOUSEMSG lastMouseMsg{};

    public:
        ~EasyxRenderer() override;

        void initalizeWindow(const Rect &rect) override;

        void setWindowTitle(const std::string& title) override;

        void update() override;

        void drawCircle(int pixel_x, int pixel_y, int radius, const Color &color) override;

        void drawFillCircle(int pixel_x, int pixel_y, int radius, const Color &color) override;

        void drawFillCircle(int pixel_x, int pixel_y, int radius, const Color &color, const Color &edge_color) override;

        void clearCircle(int pixel_x, int pixel_y, int radius) override;

        void drawRectangle(int left_up_x, int left_up_y,
                           int right_down_x, int right_down_y, const Color &color) override;

        void drawFillRectangle(int left_up_x, int left_up_y, int right_down_x, int right_down_y,
                               const Color &color) override;

        void drawFillRectangle(int left_up_x, int left_up_y, int right_down_x, int right_down_y, const Color &color,
                               const Color &edge_color) override;

        void clearRectangle(int left_up_x, int left_up_y, int right_down_x, int right_down_y) override;

        void drawLine(int pixel_start_x, int pixel_start_y,
                      int pixel_end_x, int pixel_end_y, const Color &color) override;

        void writeline(const char *str, const Rect& rect, int x, int y, const Color &color = 0) override;

        void setBackground(const Color &color) override;

        void clear() override;

        void mouseHandle(const MouseEvent &evt) override;

        bool isClosed() override;

        static void shutdown();
    };
}

#endif //KIRCUTE_PHYSICS_CPP_EASYX_H