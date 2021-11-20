#include "quadtree_store.h"

using namespace kircute_physics;

int QuadtreeEntrance::MaxLayer = 3;
int QuadtreeEntrance::DivideCount = 5;
bool Branch::DrawBranchEdge = false;

int NodeProxy::Count = 0;
int Node::Count = 0;

QuadtreeEntrance::QuadtreeEntrance(float validRange, const Rect &range) : RigidbodyContainer(validRange, range) {
    root = new NodeProxy(this, 0, Vec2f(-validRange, -validRange),
                         Vec2f((float) range.width + validRange, (float) range.height + validRange));
}

QuadtreeEntrance::QuadtreeEntrance(float validRange) : RigidbodyContainer(validRange) {
    root = new NodeProxy(this, 0, Vec2f(-validRange, -validRange),
                         Vec2f((float) range.width + validRange, (float) range.height + validRange));
}

QuadtreeEntrance::~QuadtreeEntrance() {
    delete root;
}

void QuadtreeEntrance::collide(Rigidbody *const &rbody) {
    // 运行到此处时，有关rbody的夸层碰撞检测完毕，故不进行任何操作
}

void QuadtreeEntrance::insert(Rigidbody *const &rbody) {
    delete rbody;  // 能运行到此处说明这个刚体被root认为是出界的，说明这个刚体已离开有效范围，可直接删除
}

void QuadtreeEntrance::update(const std::vector<ForceField> &field) {
    root->update(field);
    root->refactor();
}

NodeProxy::NodeProxy(Parent *const &parent, int layer, const Vec2f &start, const Vec2f &end) {
    node = new Leaf(parent, this, layer, start, end);
    Count++;
}

NodeProxy::~NodeProxy() {
    Count--;
    delete node;
}

void NodeProxy::rebuild(Node *const &n) {
    delete node;
    node = n;
}

Node::Node(Parent *const &parent, NodeProxy *const &proxy, int layer, const Vec2f &start, const Vec2f end) :
        parent(parent),
        proxy(proxy),
        layer(layer),
        start(start),
        end(end) {
    Count++;
}

Node::~Node() {
    Count--;
    for (auto rbody : storage) delete rbody;
}

void Node::insert(Rigidbody *const &rbody) {
    storage.push_back(rbody);
}

bool Node::contains(const Rigidbody *const &rbody) {
    return rbody->getPosition().x + rbody->getRadius() <= end.x &&
           rbody->getPosition().x - rbody->getRadius() >= start.x &&
           rbody->getPosition().y + rbody->getRadius() <= end.y &&
           rbody->getPosition().y - rbody->getRadius() >= start.y;
}

void Node::collideCheck() {
    for (unsigned int a = 0; a < storage.size(); a++) {
        for (unsigned int b = a + 1; b < storage.size(); b++) {
            if (Rigidbody::collide(storage[a], storage[b])) {
                Rigidbody::solve(storage[a], storage[b]);
            }
        }
    }
    for (auto rbody : storage) parent->collide(rbody);
}

void Node::collide(Rigidbody *const &rbody) {
    for (auto itr : storage)
        if (Rigidbody::collide(itr, rbody))
            Rigidbody::solve(itr, rbody);
    parent->collide(rbody);
}

void Node::edgeCheck(float left, float top, float right, float bottom) {
    for (auto rbody : storage) Rigidbody::edgeCheck(rbody, left, top, right, bottom);
}

void Node::update(const std::vector<ForceField> &field) {
    for (auto rbody : storage) {
        for (const auto &force : field) rbody->force += force(rbody);
        rbody->update();
    }
}

void Node::render(Renderer *const &renderer) const {
    for (auto rbody : storage) rbody->render(renderer);
}

void Node::refactor() {
    auto itr = storage.begin();
    while (itr != storage.end()) {
        if ((*itr)->destroy || !contains(*itr)) {
            parent->insert(*itr);
            itr = storage.erase(itr);
        } else itr++;
    }
}

Leaf::Leaf(Parent *const &parent, NodeProxy *const &proxy, int layer, const Vec2f &start, const Vec2f end) :
        Node(parent, proxy, layer, start, end) {}

void Leaf::refactor() {
    if (layer < QuadtreeEntrance::MaxLayer && storage.size() > QuadtreeEntrance::DivideCount) {
        auto p = proxy;
        p->rebuild(new Branch(parent, proxy, layer, start, end, storage));
        // Danger: 此时this已经完成析构
        p->refactor();
    } else Node::refactor();
}

Branch::Branch(Parent *const &parent, NodeProxy *const &proxy, int layer, const Vec2f &start, const Vec2f end,
               std::vector<Rigidbody *> &storage) :
        Node(parent, proxy, layer, start, end) {
    // Danger: storage所在节点可能面临析构
    for (auto item : storage) this->storage.push_back(item);
    storage.clear();  // 阻止storage析构造成刚体被意外删除

    auto core = Vec2f((start.x + end.x) / 2.0f, (start.y + end.y) / 2.0f);
    lu = new NodeProxy(proxy, layer + 1, start, core);
    ru = new NodeProxy(proxy, layer + 1, Vec2f(core.x, start.y), Vec2f(end.x, core.y));
    ld = new NodeProxy(proxy, layer + 1, Vec2f(start.x, core.y), Vec2f(core.x, end.y));
    rd = new NodeProxy(proxy, layer + 1, core, end);
}

Branch::~Branch() {
    delete lu;
    delete ru;
    delete ld;
    delete rd;
}

void Branch::insert(Rigidbody *const &rbody) {
    if (DrawBranchEdge) rbody->color.code = 0x7F7F7F;
    Node::insert(rbody);
}

void Branch::collideCheck() {
    lu->collideCheck();
    ru->collideCheck();
    ld->collideCheck();
    rd->collideCheck();
    Node::collideCheck();
}

void Branch::edgeCheck(float left, float top, float right, float bottom) {
    Node::edgeCheck(left, top, right, bottom);
    lu->edgeCheck(left, top, right, bottom);
    ru->edgeCheck(left, top, right, bottom);
    ld->edgeCheck(left, top, right, bottom);
    rd->edgeCheck(left, top, right, bottom);
}

void Branch::update(const std::vector<ForceField> &field) {
    Node::update(field);
    lu->update(field);
    ru->update(field);
    ld->update(field);
    rd->update(field);
}

void Branch::render(Renderer *const &renderer) const {
    Node::render(renderer);
    lu->render(renderer);
    ru->render(renderer);
    ld->render(renderer);
    rd->render(renderer);
    if (DrawBranchEdge) {
        renderer->drawLine((int) ((start.x + end.x) / 2.0f), (int) start.y,
                           (int) ((start.x + end.x) / 2.0f), (int) end.y, 0x007F00);
        renderer->drawLine((int) start.x, (int) ((start.y + end.y) / 2.0f),
                           (int) end.x, (int) ((start.y + end.y) / 2.0f), 0x007F00);
    }
}

void Branch::refactor() {
    auto itr = storage.begin();
    while (itr != storage.end()) {
        if (lu->contains(*itr)) {
            if (DrawBranchEdge) (*itr)->color.code = 0xFF7F7F;
            lu->insert(*itr);
            itr = storage.erase(itr);
        } else if (ld->contains(*itr)) {
            if (DrawBranchEdge) (*itr)->color.code = 0x7F7FFF;
            ld->insert(*itr);
            itr = storage.erase(itr);
        } else if (ru->contains(*itr)) {
            if (DrawBranchEdge) (*itr)->color.code = 0x7FFF7F;
            ru->insert(*itr);
            itr = storage.erase(itr);
        } else if (rd->contains(*itr)) {
            if (DrawBranchEdge) (*itr)->color.code = 0x7FFFFF;
            rd->insert(*itr);
            itr = storage.erase(itr);
        } else itr++;
    }
    lu->refactor();
    ld->refactor();
    ru->refactor();
    rd->refactor();
    Node::refactor();
}