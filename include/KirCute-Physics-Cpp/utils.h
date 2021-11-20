#ifndef KIRCUTE_PHYSICS_CPP_UTILS_H
#define KIRCUTE_PHYSICS_CPP_UTILS_H

#include <cmath>

#define F_EQUALS_F(A, B) (fabsf(A - B) < 1e-4)
#define F_EQUALS(A, B)   (fabs(A - B) < 1e-4)

namespace kircute_physics {
    struct Rect {
        int width;
        int height;

        Rect(int w, int h) : width(w), height(h) {}
    };

    struct Vec2f {
        float x;
        float y;

        explicit Vec2f(float x = .0f, float y = .0f) : x(x), y(y) {}

        Vec2f operator+(const Vec2f &o) const { return Vec2f(x + o.x, y + o.y); }

        Vec2f operator-(const Vec2f &o) const { return Vec2f(x - o.x, y - o.y); }

        Vec2f operator*(float o) const { return Vec2f(x * o, y * o); }

        Vec2f operator/(float o) const { return Vec2f(x / o, y / o); }

        Vec2f operator+() const { return Vec2f(x, y); }

        Vec2f operator-() const { return Vec2f(-x, -y); }

        Vec2f operator~() const { return Vec2f(y, x); }

        Vec2f &operator+=(const Vec2f &o) {
            x += o.x, y += o.y;
            return *this;
        }

        Vec2f &operator-=(const Vec2f &o) {
            x -= o.x, y -= o.y;
            return *this;
        }

        Vec2f &operator*=(float o) {
            x *= o, y *= o;
            return *this;
        }

        Vec2f &operator/=(float o) {
            x /= o, y /= o;
            return *this;
        }

        float operator*(const Vec2f &o) const { return x * o.x + y * o.y; } //内积
        float operator%(const Vec2f &o) const { return x * o.y - y * o.x; } //外积
        bool operator==(const Vec2f &o) const { return F_EQUALS_F(x, o.x) && F_EQUALS_F(y, o.y); }

        bool operator!=(const Vec2f &o) const { return !(o == *this); }

        Vec2f operator<<(float o) const { //逆时针旋转
            auto c = cos(o), s = sin(o);
            return Vec2f(x * c - y * s, x * s + y * c);
        }

        Vec2f operator>>(float o) const { return *this << -o; } //顺时针旋转
        float operator^(const Vec2f &o) const { //求夹角
            auto m = magnitude() * o.magnitude();
            return F_EQUALS_F(m, .0f) ? .0f : (*this * o / m);
        }

        float magnitudeSqr() const { return x * x + y * y; }

        float magnitude() const { return sqrt(magnitudeSqr()); }

        Vec2f normalize() const { return *this / magnitude(); }

        void decompose_v(const Vec2f &dir, Vec2f &out_a, Vec2f &out_b) const {
            auto in = *this ^dir;
            out_a = dir.normalize() * magnitude() * in;
            out_b = *this - out_a;
        }

        void parse_array(float *arr) const { arr[0] = x, arr[1] = y; }
    };

    struct Color {
        unsigned int code;

        Color(unsigned int code) : code(code) {}

        Color(unsigned int r, unsigned int g, unsigned int b) {
            code = 0;
            code += r * 0x10000;
            code += g * 0x100;
            code += b;
        }

        Color(float r, float g, float b) {
            code = 0;
            code += static_cast<int>(r * 0xFF) * 0x10000;
            code += static_cast<int>(g * 0xFF) * 0x100;
            code += static_cast<int>(b * 0xFF);
        }

        static Color FromHSV(float h, float s, float v) {
            while (h < .0f) h += 360.0f;
            while (h > 360.0f) h -= 360.0f;
            float r = v, g = v, b = v;
            if (!F_EQUALS_F(s, .0f)) {
                h /= 60.0f;
                auto i = static_cast<int>(h);
                auto c = h - i;
                auto x = v * (1.0f - s);
                auto y = v * (1.0f - s * c);
                auto z = v * (1.0f - s * (1.0f - c));
                switch (i) {
                    case 0:
                        r = v, g = z, b = x;
                        break;
                    case 1:
                        r = y, g = v, b = x;
                        break;
                    case 2:
                        r = x, g = v, b = z;
                        break;
                    case 3:
                        r = x, g = y, b = v;
                        break;
                    case 4:
                        r = z, g = x, b = v;
                        break;
                    case 5:
                        r = v, g = x, b = y;
                        break;
                    default:
                        r = v, g = v, b = v;
                        break;
                }
            }
            return {r, g, b};
        }

        void parse_int(unsigned int &r, unsigned int &g, unsigned int &b) const {
            r = (code & 0xFF0000u) >> 16;
            g = (code & 0xFF00u) >> 8;
            b = (code & 0xFFu);
        }

        void parse_float(float &r, float &g, float &b) const {
            unsigned int ir, ig, ib;
            parse_int(ir, ig, ib);
            r = static_cast<float>(ir) / 255.0f;
            g = static_cast<float>(ig) / 255.0f;
            b = static_cast<float>(ib) / 255.0f;
        }
    };
}

#endif //KIRCUTE_PHYSICS_CPP_UTILS_H
