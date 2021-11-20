#include "easyx_renderer.h"
#include <conio.h>

using namespace kircute_physics;

EasyxRenderer::~EasyxRenderer() {
    shutdown();
}

void EasyxRenderer::initalizeWindow(const Rect &rect) {
    initgraph(rect.width, rect.height);
    BeginBatchDraw();
}

void EasyxRenderer::update() {
    FlushBatchDraw();
}

void EasyxRenderer::drawCircle(int pixel_x, int pixel_y, int radius, const Color &color) {
    setlinecolor(color.code);
    circle(pixel_x, pixel_y, radius);
}

void EasyxRenderer::drawFillCircle(int pixel_x, int pixel_y, int radius, const Color &color) {
    setfillcolor(color.code);
    solidcircle(pixel_x, pixel_y, radius);
}

void EasyxRenderer::drawFillCircle(int pixel_x, int pixel_y, int radius, const Color &color, const Color &edge_color) {
    setlinecolor(edge_color.code);
    setfillcolor(color.code);
    fillcircle(pixel_x, pixel_y, radius);
}

void
EasyxRenderer::drawRectangle(int left_up_x, int left_up_y, int right_down_x, int right_down_y, const Color &color) {
    setlinecolor(color.code);
    rectangle(left_up_x, left_up_y, right_down_x, right_down_y);
}

void
EasyxRenderer::drawFillRectangle(int left_up_x, int left_up_y, int right_down_x, int right_down_y, const Color &color) {
    setfillcolor(color.code);
    solidrectangle(left_up_x, left_up_y, right_down_x, right_down_y);
}

void
EasyxRenderer::drawFillRectangle(int left_up_x, int left_up_y, int right_down_x, int right_down_y, const Color &color,
                                 const Color &edge_color) {
    setlinecolor(edge_color.code);
    setfillcolor(color.code);
    fillrectangle(left_up_x, left_up_y, right_down_x, right_down_y);
}

void
EasyxRenderer::drawLine(int pixel_start_x, int pixel_start_y, int pixel_end_x, int pixel_end_y, const Color &color) {
    setlinecolor(color.code);
    line(pixel_start_x, pixel_start_y, pixel_end_x, pixel_end_y);
}

void EasyxRenderer::setBackground(const Color &color) {
    setbkcolor(color.code);
}

void EasyxRenderer::clear() {
    cleardevice();
}

void EasyxRenderer::mouseHandle(const MouseEvent &evt) {
    auto m = MouseHit() ? GetMouseMsg() : lastMouseMsg;
    MouseMsg msg{};
    msg.ctrl = m.mkCtrl;
    msg.shift = m.mkShift;
    msg.x = m.x;
    msg.y = m.y;
    msg.l_button = m.mkLButton;
    msg.l_button_up = lastMouseMsg.mkLButton && !m.mkLButton;
    msg.l_button_down = !lastMouseMsg.mkLButton && m.mkLButton;
    msg.m_button = m.mkMButton;
    msg.m_button_up = lastMouseMsg.mkMButton && !m.mkMButton;
    msg.m_button_down = !lastMouseMsg.mkMButton && m.mkMButton;
    msg.r_button = m.mkRButton;
    msg.r_button_up = lastMouseMsg.mkRButton && !m.mkRButton;
    msg.r_button_down = !lastMouseMsg.mkRButton && m.mkRButton;
    msg.wheel = m.wheel;
    evt(msg);
    lastMouseMsg = m;
}

bool EasyxRenderer::isClosed() {
    if (_kbhit()) {
        int input = _getch();
        return input == 'q' || input == 'Q' || input == 27;
    }
    return false;
}

void EasyxRenderer::shutdown() {
    EndBatchDraw();
    closegraph();
}