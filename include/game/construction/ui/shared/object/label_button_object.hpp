#pragma once
#include "core/data/vector2.hpp"
#include "entt/entity/fwd.hpp"
#include "entt/entity/registry.hpp"
#include <functional>
#include <string>
#include <utility>
namespace Component::UI { struct Transform; };
namespace Nc { class ResourceStore; };


namespace Object
{

class LabelButton final
{
public:
    struct Data final
    {
        const entt::entity label;
        const entt::entity button;


        std::array<entt::entity, 2u> All() const
        { 
            return { label, button }; 
        };
    };


    static const Data Create(
        entt::registry &registry, 
        Nc::ResourceStore& resourceStore,
        Component::UI::Transform&& transform, 
        std::string&& display,
        std::function<void()>&& onClick
    ) noexcept;

private:
    class Label final
    {
    public:
        static const entt::entity Create(
            entt::registry& registry, 
            std::string&& label, 
            const Component::UI::Transform& transform
        ) noexcept;

    };


    class Button final
    {
    public:
        static const entt::entity Create(
            entt::registry& registry, 
            Component::UI::Transform&& transform, 
            std::function<void()>&& onClick
        ) noexcept;

    };

};

}