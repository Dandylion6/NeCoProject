#include "assemblers/ui/label_button_object.hpp"
#include "components/core/button_action_component.hpp"
#include "components/core/rendering/rectangle_component.hpp"
#include "components/core/rendering/text_component.hpp"
#include "components/core/transform_component.hpp"
#include "core/render_context.hpp"
#include "entt/entity/fwd.hpp"
#include "entt/entity/registry.hpp"
#include "utility/color_palette.hpp"
#include <functional>
#include <utility>


namespace Construct
{
    const entt::entity ButtonLabelEntity(
		entt::registry& registry, std::string&& label, const Component::UiTransform& transform
	)
	{
		const entt::entity entity = registry.create();

		registry.emplace<Component::UiTransform>(
			entity, transform.anchor, transform.origin, transform.size, transform.offset, transform.index + 1, transform.rotation
		);

		registry.emplace<Component::Text>(entity, std::move(label), RADAR_COLOR, WDXL, FontSize::Large, Alignment::Center, 4u);

		return entity;
	};


    const entt::entity ButtonEntity(
        entt::registry& registry, Component::UiTransform&& transform, std::function<void()>&& onClick
    )
    {
        const entt::entity entity = registry.create();

        registry.emplace<Component::UiTransform>(entity, std::move(transform));
        registry.emplace<Component::Rectangle>(entity, RenderContext::BACKGROUND_COLOR);
        registry.emplace<Component::ButtonAction>(entity, std::move(onClick));

        return entity;
    };
}


LabelButton Construct::LabelButtonObject(
    Component::UiTransform&& transform, 
    std::string &&display,
    std::function<void()>&& onClick,
    entt::registry &registry, 
    ResourceStore &resourceStore
)
{
    transform.size = Nc::Vector2f(420.0f, 68.0f);
    const entt::entity button = Construct::ButtonEntity(registry, std::move(transform), std::move(onClick));

    const Component::UiTransform& entityTransform = registry.get<Component::UiTransform>(button);
    const entt::entity label = Construct::ButtonLabelEntity(registry, std::move(display), entityTransform);

    LabelButton labelButton = LabelButton(label, button);
    return labelButton;
}