#include <string>
#include <memory>
#include <map>
#include <SFML/Graphics.hpp>
#include "textures.hpp"

template <typename Resource, typename Identifier>
class ResourceHolder {
   public:
    void load(Identifier id, const std::string& filename);

    template <typename Parameter>
    void load(Identifier id, const std::string& filename,
              const Parameter& secondParameter);

    Resource& get(Identifier id);
    const Resource& get(Identifier id) const;

   private:
    std::map<Identifier, std::unique_ptr<Resource>> mResourceMap;
};

typedef ResourceHolder<sf::Texture, Textures::ID> TextureHolder;

#include "resource_holder.inl"
