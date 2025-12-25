#pragma once
#include <concepts>
#include <functional>
#include <vector>

#include "renderable.hpp"

namespace widget {

template <typename T>
concept HasToString = requires(const T& obj) {
    { obj.to_string() } -> std::same_as<std::string>;
};
template <HasToString T>
class ListView : public Renderable,
                 public sf::Transformable,
                 public sf::Drawable {
   public:
    ListView(const std::vector<T>& item_list);

    void on_item_click(std::function<void(const T&)>);

    // Renderable
    void render(sf::RenderWindow& w) override;
    void handle_click_event(sf::RenderWindow& w, sf::Event e) override;

    // Drawable
    void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

   private:
    const std::vector<T>& mItemList;
    sf::RectangleShape mEntryRect;
	sf::Text mEntryText;
	sf::Font mFont;
    std::function<void(const T&)> mClickItemCallbackFunc;
};

};  // namespace widget

#include "../../src/widget/list_view.tpp"  // FUCK C++
