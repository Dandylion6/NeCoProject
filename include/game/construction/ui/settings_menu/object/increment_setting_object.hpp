#pragma once
#include "core/data/vector2.hpp"
#include "entt/entity/entity.hpp"
#include "entt/entity/fwd.hpp"
#include "game/component/core/rendering/text_component.hpp"
#include "game/component/ui/increment_component.hpp"
#include <array>
#include <string>
namespace Nc { class ResourceStore; };


namespace Object
{

class IncrementSetting final
{
public:
    struct Data final
    {
        entt::entity label = entt::null;
        entt::entity valueDisplay = entt::null;
        entt::entity decreaseButton = entt::null;
        entt::entity increaseButton = entt::null;


        std::array<entt::entity, 4u> All() const 
        { 
            return { label, valueDisplay, decreaseButton, increaseButton }; 
        };
    };


    static const Data Create(
        entt::registry& registry, 
        Nc::ResourceStore& resourceStore,
        std::string&& display,
        Component::UI::Increment&& increment,
        Nc::Vector2f position
    ) noexcept;

private:
    class Label final
    {
    public:
        static entt::entity Create(
            entt::registry& registry,
            std::string&& display,
            Nc::Vector2f position
        ) noexcept;     

    };


    class ValueDisplay final
    {
    public:
        static entt::entity Create(
            entt::registry& registry,
            Component::UI::Increment&& increment,
            Nc::Vector2f position,
            Nc::Vector2f offset
        ) noexcept;

    };


    class IncreaseButton final
    {
    public:
        static entt::entity Create(
            entt::registry& registry,
            Component::UI::Increment& increment,
            Component::Text& valueDisplay,
            Nc::Vector2f position,
            Nc::Vector2f offset
        ) noexcept;    

    };


    class DecreaseButton final
    {
    public:
        static entt::entity Create(
            entt::registry& registry,
            Component::UI::Increment& increment, 
            Component::Text& valueDisplay,
            Nc::Vector2f position,
            Nc::Vector2f offset
        ) noexcept;

    };

};

}