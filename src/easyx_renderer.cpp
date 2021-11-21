#include "easyx_renderer.h"
#include <conio.h>
#include <iostream>

using namespace kircute_physics;

EasyxRenderer::~EasyxRenderer() {
    shutdown();
    std::cout << "Deleted Renderer." << std::endl;
}

void EasyxRenderer::initalizeWindow(const Rect &rect) {
    initgraph(rect.width, rect.height, EW_NOCLOSE);
    BeginBatchDraw();
}

void EasyxRenderer::setWindowTitle(const std::string &title) {
    SetWindowText(GetHWnd(), title.c_str());
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

void EasyxRenderer::clearCircle(int pixel_x, int pixel_y, int radius) {
    clearcircle(pixel_x, pixel_y, radius);
}

void EasyxRenderer::drawRectangle(int left_up_x, int left_up_y,
                                  int right_down_x, int right_down_y, const Color &color) {
    setlinecolor(color.code);
    rectangle(left_up_x, left_up_y, right_down_x, right_down_y);
}

void EasyxRenderer::drawFillRectangle(int left_up_x, int left_up_y,
                                      int right_down_x, int right_down_y, const Color &color) {
    setfillcolor(color.code);
    solidrectangle(left_up_x, left_up_y, right_down_x, right_down_y);
}

void EasyxRenderer::drawFillRectangle(int left_up_x, int left_up_y,
                                      int right_down_x, int right_down_y,
                                      const Color &color, const Color &edge_color) {
    setlinecolor(edge_color.code);
    setfillcolor(color.code);
    fillrectangle(left_up_x, left_up_y, right_down_x, right_down_y);
}

void EasyxRenderer::clearRectangle(int left_up_x, int left_up_y, int right_down_x, int right_down_y) {
    clearrectangle(left_up_x, left_up_y, right_down_x, right_down_y);
}

void EasyxRenderer::drawLine(int pixel_start_x, int pixel_start_y,
                             int pixel_end_x, int pixel_end_y, const Color &color) {
    setlinecolor(color.code);
    line(pixel_start_x, pixel_start_y, pixel_end_x, pixel_end_y);
}

void EasyxRenderer::writeline(const char *str, const Rect& rect, int x, int y, const Color &color) {
    settextcolor(color.code);
    RECT r = { x, y, rect.width + x, rect.height + y };
    drawtext(str, &r, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
}

void EasyxRenderer::setBackground(const Color &color) {
    setbkcolor(color.code);
}

void EasyxRenderer::clear() {
    cleardevice();
}

void EasyxRenderer::mouseHandle(const MouseEvent &evt) {
    auto m = lastMouseMsg;
    while (MouseHit()) m = GetMouseMsg();
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
    lastMouseMsg.wheel = 0;
}

bool EasyxRenderer::isClosed() {
    if (_kbhit()) {
        int input = _getch();
        return input == 'q' || input == 'Q' || input == 27;
    }
    return closed;
}

void EasyxRenderer::shutdown() {
    EndBatchDraw();
    closegraph();
}