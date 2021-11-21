#ifndef KIRCUTE_PHYSICS_CPP_QUADTREE_STORE_H
#define KIRCUTE_PHYSICS_CPP_QUADTREE_STORE_H

#include "store.h"

namespace kircute_physics {
    class QuadtreeEntrance;
    class Parent;
    class Node;
    class NodeProxy;
    class Leaf;
    class Branch;

    class Parent {
    protected:
        virtual ~Parent() = default;

        virtual void collide(Rigidbody *const &rbody) = 0;

        virtual void insert(Rigidbody *const &rbody) = 0;

        friend Node;
    };

    class Node {
    protected:
        std::vector<Rigidbody *> storage;
        Parent *parent;
        NodeProxy *proxy;
        int layer;
        Vec2f start;
        Vec2f end;

        Node(Parent *const &parent,
             NodeProxy *const &proxy,
             int layer,
             const Vec2f &start,
             Vec2f end);

        virtual ~Node();

        virtual void insert(Rigidbody *const &rbody);

        virtual bool contains(const Rigidbody *const &rbody);

        virtual void collideCheck();

        virtual void collide(Rigidbody *const &rbody);

        virtual void edgeCheck(float left, float top, float right, float bottom);

        virtual void update(const std::vector<ForceField> &field);

        virtual void render(Renderer *const &renderer) const;

        virtual void refactor();

        virtual void clear();

        friend NodeProxy;

    public:
        static int Count;
    };

    class Leaf : public Node {
    protected:
        Leaf(Parent *const &parent,
             NodeProxy *const &proxy,
             int layer,
             const Vec2f &start,
             Vec2f end);

        void refactor() override;

        friend NodeProxy;
    };

    class Branch : public Node {
    private:
        NodeProxy *lu;
        NodeProxy *ld;
        NodeProxy *ru;
        NodeProxy *rd;

        Branch(Parent *const &parent,
               NodeProxy *const &proxy,
               int layer,
               const Vec2f &start,
               Vec2f end,
               std::vector<Rigidbody *> &storage);

        ~Branch() override;

        friend Leaf;

    protected:
        void insert(Rigidbody *const &rbody) override;

        void collideCheck() override;

        void edgeCheck(float left, float top, float right, float bottom) override;

        void update(const std::vector<ForceField> &field) override;

        void render(Renderer *const &renderer) const override;

        void refactor() override;

        void clear() override;

    public:
        static bool DrawBranchEdge;
    };

    class NodeProxy : public Parent {
    private:
        Node *node;

        void rebuild(Node *const &n);

        friend Leaf;

    public:
        NodeProxy(Parent *const &parent,
                  int layer,
                  const Vec2f &start,
                  const Vec2f &end);

        ~NodeProxy() override;

        static int Count;

        void insert(Rigidbody *const &rbody) override { node->insert(rbody); }

        bool contains(const Rigidbody *const &rbody) { return node->contains(rbody); }

        void collideCheck() { node->collideCheck(); }

        void collide(Rigidbody *const &rbody) override { node->collide(rbody); }

        void edgeCheck(float left, float top, float right, float bottom) { node->edgeCheck(left, top, right, bottom); }

        void update(const std::vector<ForceField> &field) { node->update(field); }

        void render(Renderer *const &renderer) const { node->render(renderer); }

        void refactor() { node->refactor(); }

        void clear() { node->clear(); }
    };

    class QuadtreeEntrance : public RigidbodyContainer, public Parent {
    private:
        NodeProxy *root;

    protected:
        void collide(Rigidbody *const &rbody) override;

        void insert(Rigidbody *const &rbody) override;

    public:
        QuadtreeEntrance(float validRange, const Rect &range);

        explicit QuadtreeEntrance(float validRange);

        ~QuadtreeEntrance() override;

        static int MaxLayer;
        static int DivideCount;

        void push_back(Rigidbody *const &rbody) override { root->insert(rbody); }

        void update(const std::vector<ForceField> &field) override;

        void collideCheck() override { root->collideCheck(); }

        void edgeCheck(float left, float top, float right, float bottom) override {
            root->edgeCheck(left, top, right, bottom);
        }

        void render(Renderer *const &renderer) const override { root->render(renderer); }

        void clear() override;
    };
}

#endif //KIRCUTE_PHYSICS_CPP_QUADTREE_STORE_H