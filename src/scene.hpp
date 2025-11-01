#include <memory>
#include <vector>
#include <SFML/Graphics/Transformable.hpp>
#include <SFML/Graphics/Drawable.hpp>
#include <SFML/Graphics/RenderStates.hpp>

class SceneNode : public sf::Transformable, public sf::Drawable {
   public:
    typedef std::unique_ptr<SceneNode> SceneNodeSP;

   public:
    SceneNode();
    SceneNode(const SceneNode&) = delete;
    SceneNode& operator=(const SceneNode&) = delete;

    void attachChild(SceneNodeSP child);
    SceneNodeSP detachChild(const SceneNode& child);

   private:
    std::vector<SceneNodeSP> mChildren;
    SceneNode* mParent;

    virtual void draw(sf::RenderTarget& target,
                      sf::RenderStates states) const override final;
    virtual void drawCurrent(sf::RenderTarget& target,
                             sf::RenderStates states) const = 0;
};
