#pragma once
#include <array>
#include <string>

#include "core/data/vector2.hpp"
#include "entt/entity/entity.hpp"
#include "entt/entity/fwd.hpp"
#include "game/component/core/rendering/text_component.hpp"
#include "game/component/ui/increment_component.hpp"
#include "game/contexts/scene_context.hpp"


namespace Object
{

class IncrementSetting final
{
public:
    // ------ Types ------

    struct Data final
    {
        entt::entity label = entt::null;
        entt::entity valueDisplay = entt::null;
        entt::entity decreaseButton = entt::null;
        entt::entity increaseButton = entt::null;


        [[nodiscard]] std::array<entt::entity, 4u> All() const
        {
            return { label, valueDisplay, decreaseButton, increaseButton };
        };
    };


    // ------ Functions ------

    static Data Create(
        const SceneContext& context,
        std::string&& display,
        const Component::UI::Increment& increment,
        Nc::Vector2f position
    ) noexcept;

private:
    // ------ Types ------

    struct IncrementContext final
    {
        entt::registry& registry;
        Nc::Vector2f position;
        Nc::Vector2f offset;
        entt::entity increment;


        IncrementContext(
            entt::registry& registry,
            const Nc::Vector2f position,
            const Nc::Vector2f offset,
            const entt::entity increment
        ) noexcept
            : registry(registry),
              position(position),
              offset(offset),
              increment(increment) { }
    };


    class Label final
    {
    public:
        // ------ Functions ------
        static entt::entity Create(entt::registry& registry, std::string&& display, Nc::Vector2f position) noexcept;

    };


    class ValueDisplay final
    {
    public:
        // ------ Functions ------
        static entt::entity Create(
            entt::registry& registry,
            Component::UI::Increment increment,
            Nc::Vector2f position,
            Nc::Vector2f offset
        ) noexcept;

    };


    class IncreaseButton final
    {
    public:
        // ------ Functions ------
        static entt::entity Create(const IncrementContext& context) noexcept;

    };


    class DecreaseButton final
    {
    public:
        // ------ Functions ------
        static entt::entity Create(const IncrementContext& context) noexcept;

    };

};

}
