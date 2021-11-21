#include "frame.h"
#include "quadtree_store.h"
#include "easyx_renderer.h"

#define DISPLAYER_LEFT          645
#define DISPLAYER_RIGHT         755
#define RADIUS_DISPLAYER_TOP    35
#define RADIUS_DISPLAYER_BOTTOM 155
#define BALL_Y                  90
#define CLEAR_BUTTON_TOP        165
#define GRAVITY_BUTTON_TOP      200
#define CLOSE_BUTTON_TOP        235
#define LEN_TEXT_TOP            5
#define BUTTON_HEIGHT           25
#define BUTTON_WIDTH            DISPLAYER_RIGHT - DISPLAYER_LEFT

using namespace kircute_physics;

class Button {
private:
    int x;
    int y;
    Rect size;
    std::string text;
    std::function<void()> action;

public:
    Button(int x, int y, Rect size, const std::string &text, std::function<void()> act = [&]() {}) :
        x(x), y(y), size(size), text(text), action(act) {}
    ~Button() = default;

    bool contains(int mx, int my) {
        return x <= mx && x + size.width >= x && y <= my && y + size.height >= my;
    }

    void setText(const std::string &text) { this->text = text; }

    void setAction(std::function<void()> action) { this->action = action; }

    void render(int mouse_x, int mouse_y, Renderer *const &renderer) {
        renderer->drawFillRectangle(x, y, x + size.width, y + size.height, 0xFFFFFF, contains(mouse_x, mouse_y) ? 0 : 0xAFAFAF);
        renderer->writeline(text.c_str(), size, x, y);
    }

    void update(const MouseMsg &msg) {
        if (contains(msg.x, msg.y) && msg.l_button_down) action();
    }
};

class Demo : public Frame {
private:
    int mousePosX{0};
    int mousePosY{0};
    float radius{10.0f};
    float mass{10.0f};
    bool enableGravity{true};
    ForceField gravity = [&](const Rigidbody *const &rbody) {
        return Vec2f(.0f, 0.5f);
    };
    Button clear_b = Button(DISPLAYER_LEFT, CLEAR_BUTTON_TOP, Rect(BUTTON_WIDTH, BUTTON_HEIGHT), "清空", [&]() {
        world->clear();
    });
    Button gravity_b = Button(DISPLAYER_LEFT, GRAVITY_BUTTON_TOP, Rect(BUTTON_WIDTH, BUTTON_HEIGHT), "关闭重力", [&]() {
        if (enableGravity) world->clearField();
        else world->addField(gravity);
        enableGravity = !enableGravity;
    });
    Button close_b = Button(DISPLAYER_LEFT, CLOSE_BUTTON_TOP, Rect(BUTTON_WIDTH, BUTTON_HEIGHT), "关闭程序", [&]() {
        renderer->closed = true;
    });

public:
    Demo() : Frame(new EasyxRenderer(), new QuadtreeEntrance(50.0f)) {}

    void onInit() override {
        Branch::DrawBranchEdge = true;
        world->addField(gravity);
        renderer->setWindowTitle("KirCute-Physics-Cpp 四叉树样例 - 按Q退出");
    }

    void onRefresh() override {
        renderer->mouseHandle([&](const MouseMsg &msg) {
            mousePosX = msg.x, mousePosY = msg.y;
            if (msg.l_button_down) {
                if (!clear_b.contains(msg.x, msg.y) && !gravity_b.contains(msg.x, msg.y) &&
                    !close_b.contains(msg.x, msg.y)) world->create(mass, radius, Vec2f(msg.x, msg.y));
            }
            clear_b.update(msg);
            gravity_b.update(msg);
            close_b.update(msg);
            radius += msg.m_button ? .0f : static_cast<float>(msg.wheel) * 0.03f;
            radius = max(radius, 3.0f);
            radius = min(radius, 40.0f);
            mass += static_cast<float>(msg.wheel) * 0.03f;
            mass = max(mass, 3.0f);
            mass = min(mass, 40.0f);
        });
    }

    void onRender() override {
        char text[20];
        renderer->drawFillRectangle(DISPLAYER_LEFT, RADIUS_DISPLAYER_TOP,
                                    DISPLAYER_RIGHT, RADIUS_DISPLAYER_BOTTOM, 0xFFFFFF, 0xAFAFAF);
        renderer->drawFillCircle((DISPLAYER_LEFT + DISPLAYER_RIGHT) / 2, BALL_Y, radius, 0xFF);
        sprintf(text, "r=%.1f  m=%.1f", radius, mass);
        renderer->writeline(text, Rect(DISPLAYER_RIGHT - DISPLAYER_LEFT, 25), DISPLAYER_LEFT, RADIUS_DISPLAYER_BOTTOM - 25);
        clear_b.render(mousePosX, mousePosY, renderer);
        gravity_b.setText(enableGravity ? "关闭重力" : "开启重力");
        gravity_b.render(mousePosX, mousePosY, renderer);
        close_b.render(mousePosX, mousePosY, renderer);
        sprintf(text, "当前数量：%d", Rigidbody::Count);
        renderer->writeline(text, Rect(DISPLAYER_RIGHT - DISPLAYER_LEFT, 25), DISPLAYER_LEFT, LEN_TEXT_TOP);
    }
};

int main() {
    auto frame = new Demo();
    frame->start();
    delete frame;
    return 0;
}
