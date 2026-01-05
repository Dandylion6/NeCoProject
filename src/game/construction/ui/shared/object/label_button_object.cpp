#include "game/construction/ui/shared/object/label_button_object.hpp"
#include "core/data/vector2.hpp"
#include "core/runtime/resource_store.hpp"
#include "entt/entity/fwd.hpp"
#include "entt/entity/registry.hpp"
#include "game/component/core/interactive/click_action_component.hpp"
#include "game/component/core/rendering/rectangle_component.hpp"
#include "game/component/core/rendering/text_component.hpp"
#include "game/component/core/transform_component.hpp"
#include "game/utility/color_palette.hpp"
#include <functional>
#include <string>
#include <utility>


const Object::LabelButton::Data Object::LabelButton::Create(
    entt::registry& registry, 
    Nc::ResourceStore& resourceStore, 
    Component::UI::Transform&& transform, 
    std::string&& display, 
    std::function<void()>&& onClick
) noexcept
{
    constexpr Nc::Vector2f DEFAULT_SIZE = Nc::Vector2f(420.0f, 68.0f);

    if (transform.size == Nc::Vector2f::Zero()) transform.size = DEFAULT_SIZE;
    
    const entt::entity button = Button::Create(registry, std::move(transform), std::move(onClick));
    const Component::UI::Transform& entityTransform = registry.get<Component::UI::Transform>(button);
    
    const entt::entity label = Label::Create(registry, std::move(display), entityTransform);

    return { label, button };
}


const entt::entity Object::LabelButton::Label::Create(
    entt::registry& registry, 
    std::string&& label, 
    const Component::UI::Transform& transform
) noexcept
{
    const entt::entity entity = registry.create();

	registry.emplace<Component::UI::Transform>(
		entity, 
        transform.anchor, 
        transform.origin, 
        transform.size, 
        transform.offset, 
        transform.index + 1, 
        transform.rotation
	);

	registry.emplace<Component::Text>(
        entity, 
        std::move(label), 
        Palette::RADAR_COLOR, 
        Nc::Font::WDXL, 
        Nc::Font::Size::Large, 
        Alignment::Center, 
        4u
    );

	return entity;
}


const entt::entity Object::LabelButton::Button::Create(
    entt::registry& registry, 
    Component::UI::Transform&& transform, 
    std::function<void()>&& onClick
) noexcept
{
    const entt::entity entity = registry.create();

    registry.emplace<Component::UI::Transform>(entity, std::move(transform));
    registry.emplace<Component::Rectangle>(entity, Palette::BACKGROUND_COLOR);
    registry.emplace<Component::Action::Click>(entity, std::move(onClick));

    return entity;
}
