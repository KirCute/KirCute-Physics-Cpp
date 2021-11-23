#include "frame.h"
#include "quadtree_store.h"
#include "easyx_renderer.h"

using namespace kircute_physics;

class Demo : public Frame {
private:
	bool respawnNeeded{true};
	bool moving;
	bool downed;
	Vec2f origin;
	Vec2f target;
	Rigidbody* white;

	void respawn() {
		respawnNeeded = false;
		white = world->create(1.0f, 10.5f, Vec2f(284.8f, 356.0f), 0xFFFFFF);
		white->dictionary.insert(std::pair<std::string, std::string>("type", "white"));
		white->triggers.push_back([&](Rigidbody *const &rbody) {
			if (strcmp(rbody->dictionary["type"].c_str(), "hole") == 0) respawnNeeded = true;
			return true;
		});
		white->onRefresh = [&](Rigidbody *const &rbody) {
			if (!F_EQUALS_F(rbody->getVelocity().magnitudeSqr(), .0f)) moving = true;
		};
	}

	void spawnHole(const Vec2f &pos) {
		auto hole = world->create(1.0f, 16.0f, pos, 0);
		hole->dictionary.insert(std::pair<std::string, std::string>("type", "hole"));
		hole->triggers.push_back([&](Rigidbody *const &rbody) {
			rbody->destroy();
			return false;
		});
	}

	void spawnBall(const Vec2f& pos, const Color &color) {
		auto ball = world->create(1.0f, 10.5f, pos, color);
		ball->dictionary.insert(std::pair<std::string, std::string>("type", "ball"));
		ball->onRefresh = [&](Rigidbody* const& rbody) {
			if (!F_EQUALS_F(rbody->getVelocity().magnitudeSqr(), .0f)) moving = true;
		};
	}

public:
	Demo() : Frame(new EasyxRenderer(), new QuadtreeEntrance(50.0f, Rect(1424, 712))) {}
	~Demo() = default;

	void onInit() override {
		renderer->setBackground(0x006F00);
		world->addField([&](const Rigidbody *const &rbody) {
			if (strcmp(rbody->dictionary.find("type")->second.c_str(), "hole") == 0) return Vec2f();
			return rbody->getVelocity().magnitude() > 0.1f ? -rbody->getVelocity().normalize() * 0.1f : -rbody->getVelocity();
		});
		spawnHole(Vec2f(.0f, .0f));
		spawnHole(Vec2f(712.0f, .0f));
		spawnHole(Vec2f(1424.0f, .0f));
		spawnHole(Vec2f(.0f, 712.0f));
		spawnHole(Vec2f(712.0f, 712.0f));
		spawnHole(Vec2f(1424.0f, 712.0f));

		const auto r = 10.5f;
		const auto sqrt_3 = 1.75f * r;
		spawnBall(Vec2f(1139.2f - 2.0f * sqrt_3, 356.0f), 0x0000AF);
		spawnBall(Vec2f(1139.2f - sqrt_3, 356.0f + r), 0x0000AF);
		spawnBall(Vec2f(1139.2f - sqrt_3, 356.0f - r), 0x0000AF);
		spawnBall(Vec2f(1139.2f, 356.0f), 0x0000AF);
		spawnBall(Vec2f(1139.2f, 356.0f + 2.0f * r), 0x0000AF);
		spawnBall(Vec2f(1139.2f, 356.0f - 2.0f * r), 0x0000AF);
		spawnBall(Vec2f(1139.2f + sqrt_3, 356.0f + r), 0x0000AF);
		spawnBall(Vec2f(1139.2f + sqrt_3, 356.0f + 3.0f * r), 0x0000AF);
		spawnBall(Vec2f(1139.2f + sqrt_3, 356.0f - r), 0x0000AF);
		spawnBall(Vec2f(1139.2f + sqrt_3, 356.0f - 3.0f * r), 0x0000AF);

		respawn();
	}

	void onRefresh() override {
		if (!moving) {
			if (respawnNeeded) respawn();
			renderer->mouseHandle([&](const MouseMsg& msg) {
				if (!downed && msg.l_button_down) {
					downed = true;
					origin = Vec2f(msg.x, msg.y);
				}
				else if (downed && msg.l_button) {
					target = Vec2f(msg.x, msg.y);
				}
				else if (downed && (msg.l_button_up || !msg.l_button)) {
					downed = false;
					auto vel = (origin - target) * 0.2f;
					if (vel.magnitudeSqr() > 50.0f * 50.0f) vel = vel.normalize() * 50.0f;
					white->appendVelocity(vel);
				}
				else origin = Vec2f(msg.x, msg.y);
			});
		}
		moving = false;
	}

	void onRender() override {
		if (moving) return;
		auto dir = origin == white->getPosition() ? Vec2f(-1.0f, .0f) : (origin - white->getPosition()).normalize();
		if (downed && origin != target) {
			dir = (target - origin) * 0.05f;
			dir = dir + dir.normalize();
			if (dir.magnitudeSqr() > 13.5f * 13.5f) dir = dir.normalize() * 13.5f;
		}
		auto start = white->getPosition() + dir * 20.0f + dir.normalize() * 20.0f;
		auto end = white->getPosition() + dir * 20.0f + dir.normalize() * 500.0f;
		renderer->drawLine(start.x, start.y, end.x, end.y, 0xFFFFFF);
	}
};

int main() {
	auto frame = new Demo();
	frame->start();
	delete frame;
	return 0;
}