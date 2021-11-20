#ifndef KIRCUTE_PHYSICS_CPP_RENDER_H
#define KIRCUTE_PHYSICS_CPP_RENDER_H

#include "utils.h"
#include <functional>

namespace kircute_physics {
    struct MouseMsg {
        bool ctrl;
        bool shift;
        bool l_button;
        bool l_button_up;
        bool l_button_down;
        bool m_button;
        bool m_button_up;
        bool m_button_down;
        bool r_button;
        bool r_button_up;
        bool r_button_down;
        short wheel;
        int x;
        int y;
    };

    typedef std::function<void(const MouseMsg &)> MouseEvent;

    class Renderer {
    public:
        virtual ~Renderer() = default;

        virtual void initalizeWindow(const Rect &rect) = 0;

        virtual void update() {}

        virtual void drawCircle(int pixel_x, int pixel_y, int radius, const Color &color) = 0;

        virtual void drawFillCircle(int pixel_x, int pixel_y, int radius, const Color &color) = 0;

        virtual void drawFillCircle(int pixel_x, int pixel_y, int radius,
                                    const Color &color, const Color &edge_color) = 0;

        virtual void drawRectangle(int left_up_x, int left_up_y,
                                   int right_down_x, int right_down_y, const Color &color) = 0;

        virtual void drawFillRectangle(int left_up_x, int left_up_y,
                                       int right_down_x, int right_down_y, const Color &color) = 0;

        virtual void drawFillRectangle(int left_up_x, int left_up_y,
                                       int right_down_x, int right_down_y,
                                       const Color &color, const Color &edge_color) = 0;

        virtual void drawLine(int pixel_start_x, int pixel_start_y,
                              int pixel_end_x, int pixel_end_y, const Color &color) = 0;

        virtual void setBackground(const Color &color) = 0;

        virtual void clear() = 0;

        virtual void mouseHandle(const MouseEvent &evt) = 0;

        virtual bool isClosed() = 0;
    };
}

#endif //KIRCUTE_PHYSICS_CPP_RENDER_H