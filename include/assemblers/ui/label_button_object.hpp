#pragma once
#include "components/core/button_action_component.hpp"
#include "components/core/rendering/rectangle_component.hpp"
#include "components/core/rendering/text_component.hpp"
#include "components/core/transform_component.hpp"
#include "core/resource_store.hpp"
#include "entt/entity/fwd.hpp"
#include "entt/entity/registry.hpp"
#include "utility/color.hpp"
#include "utility/color_palette.hpp"
#include "utility/vector2.hpp"
#include <functional>
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
		entt::registry& registry, std::string&& label, const Component::UiTransform& transform
	)
	{
		const entt::entity entity = registry.create();

		registry.emplace<Component::UiTransform>(
			entity, transform.anchor, transform.origin, transform.size, transform.offset, transform.index + 1, transform.rotation
		);

		registry.emplace<Component::Text>(entity, std::move(label), Palette::RADAR_COLOR, WDXL, FontSize::Large, Alignment::Center, 4u);

		return entity;
	};


    inline const entt::entity ButtonEntity(
        entt::registry& registry, Component::UiTransform&& transform, std::function<void()>&& onClick
    )
    {
        const entt::entity entity = registry.create();

        registry.emplace<Component::UiTransform>(entity, std::move(transform));
        registry.emplace<Component::Rectangle>(entity, Palette::BACKGROUND_COLOR);
        registry.emplace<Component::ButtonAction>(entity, std::move(onClick));

        return entity;
    };

    template<class T>
    inline LabelButton LabelButtonObject(
        Component::UiTransform&& transform, 
        std::string &&display,
        std::function<void()>&& onClick,
        entt::registry &registry, 
        ResourceStore& resourceStore
    )
    {
        transform.size = Nc::Vector2f(420.0f, 68.0f);
        const entt::entity button = Construct::ButtonEntity(registry, std::move(transform), std::move(onClick));

        const Component::UiTransform& entityTransform = registry.get<Component::UiTransform>(button);
        const entt::entity label = Construct::ButtonLabelEntity(registry, std::move(display), entityTransform);

        registry.emplace<T>(button);
        registry.emplace<T>(label);

        LabelButton labelButton = LabelButton(label, button);
        return labelButton;
    }


    inline LabelButton LabelButtonObject(
        Component::UiTransform&& transform, 
        std::string &&display,
        std::function<void()>&& onClick,
        entt::registry &registry, 
        ResourceStore& resourceStore
    )
    {
        if (transform.size == Nc::Vector2f::Zero()) transform.size = Nc::Vector2f(420.0f, 68.0f);
        const entt::entity button = Construct::ButtonEntity(registry, std::move(transform), std::move(onClick));

        const Component::UiTransform& entityTransform = registry.get<Component::UiTransform>(button);
        const entt::entity label = Construct::ButtonLabelEntity(registry, std::move(display), entityTransform);

        LabelButton labelButton = LabelButton(label, button);
        return labelButton;
    }
}