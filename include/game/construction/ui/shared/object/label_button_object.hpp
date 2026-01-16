#pragma once
#include <functional>
#include <string>

#include "entt/entity/fwd.hpp"
#include "entt/entity/registry.hpp"
#include "game/contexts/scene_context.hpp"


namespace Component::UI { struct Transform; };


namespace Object
{

class LabelButton final
{
public:
    struct Data final
    {
        entt::entity label;
        entt::entity button;


        [[nodiscard]] std::array<entt::entity, 2u> All() const
        {
            return { label, button };
        };
    };


    static Data Create(
        const SceneContext& context,
        Component::UI::Transform& transform,
        std::string&& display,
        std::function<void()>&& onClick
    ) noexcept;

private:
    class Label final
    {
    public:
        static entt::entity Create(
            entt::registry& registry,
            std::string&& label,
            const Component::UI::Transform& transform
        ) noexcept;

    };


    class Button final
    {
    public:
        static entt::entity Create(
            entt::registry& registry,
            Component::UI::Transform transform,
            std::function<void()>&& onClick
        ) noexcept;

    };

};

}
