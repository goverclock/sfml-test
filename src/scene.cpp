#include "scene.hpp"
#include <algorithm>
#include <cassert>

void SceneNode::attachChild(SceneNodeSP child) {
    child->mParent = this;
    mChildren.push_back(std::move(child));
}

SceneNode::SceneNodeSP SceneNode::detachChild(const SceneNode& child) {
    auto found =
        std::find_if(mChildren.begin(), mChildren.end(),
                     [&](SceneNodeSP& p) -> bool { return p.get() == &child; });
    assert(found != mChildren.end());
    SceneNodeSP ret = std::move(*found);
    ret->mParent = nullptr;
    mChildren.erase(found);
    return ret;
}

void SceneNode::draw(sf::RenderTarget& target, sf::RenderStates states) const {
    states.transform *= getTransform();
    drawCurrent(target, states);

    for (const auto& c : mChildren) c->drawCurrent(target, states);
}
