#pragma once
#include "game/component/core/interactive/button_action_component.hpp"
#include "game/component/core/rendering/rectangle_component.hpp"
#include "game/component/core/rendering/text_component.hpp"
#include "game/component/core/transform_component.hpp"
#include "core/runtime/resource_store.hpp"
#include "entt/entity/fwd.hpp"
#include "entt/entity/registry.hpp"
#include "game/utility/color_palette.hpp"
#include "core/data/vector2.hpp"
#include <functional>
#include <string>
#include <utility>


struct LabelButton
{
    const entt::entity label;
    const entt::entity button;

    LabelButton(const entt::entity label, const entt::entity button): label(label), button(button) { }; 
};


namespace Construct 
{
    inline const entt::entity ButtonLabelEntity(
		entt::registry& registry, std::string&& label, const Component::UI::Transform& transform
	)
	{
		const entt::entity entity = registry.create();

		registry.emplace<Component::UI::Transform>(
			entity, transform.anchor, transform.origin, transform.size, transform.offset, transform.index + 1, transform.rotation
		);

		registry.emplace<Component::Text>(entity, std::move(label), Palette::RADAR_COLOR, Nc::Font::WDXL, Nc::Font::Size::Large, Alignment::Center, 4u);

		return entity;
	};


    inline const entt::entity ButtonEntity(
        entt::registry& registry, Component::UI::Transform&& transform, std::function<void()>&& onClick
    )
    {
        const entt::entity entity = registry.create();

        registry.emplace<Component::UI::Transform>(entity, std::move(transform));
        registry.emplace<Component::Rectangle>(entity, Palette::BACKGROUND_COLOR);
        registry.emplace<Component::ButtonAction>(entity, std::move(onClick));

        return entity;
    };

    template<class... T>
    inline LabelButton LabelButtonObject(
        Component::UI::Transform&& transform, 
        std::string &&display,
        std::function<void()>&& onClick,
        entt::registry &registry, 
        Nc::ResourceStore& resourceStore
    )
    {
        transform.size = Nc::Vector2f(420.0f, 68.0f);
        const entt::entity button = Construct::ButtonEntity(registry, std::move(transform), std::move(onClick));

        const Component::UI::Transform& entityTransform = registry.get<Component::UI::Transform>(button);
        const entt::entity label = Construct::ButtonLabelEntity(registry, std::move(display), entityTransform);

        (registry.emplace<T>(button), ...);
        (registry.emplace<T>(label), ...);

        LabelButton labelButton = LabelButton(label, button);
        return labelButton;
    }


    inline LabelButton LabelButtonObject(
        Component::UI::Transform&& transform, 
        std::string &&display,
        std::function<void()>&& onClick,
        entt::registry &registry, 
        Nc::ResourceStore& resourceStore
    )
    {
        if (transform.size == Nc::Vector2f::Zero()) transform.size = Nc::Vector2f(420.0f, 68.0f);
        const entt::entity button = Construct::ButtonEntity(registry, std::move(transform), std::move(onClick));

        const Component::UI::Transform& entityTransform = registry.get<Component::UI::Transform>(button);
        const entt::entity label = Construct::ButtonLabelEntity(registry, std::move(display), entityTransform);

        LabelButton labelButton = LabelButton(label, button);
        return labelButton;
    }
}