#include "game/construction/ui/shared/object/label_button_object.hpp"

#include <string>
#include <utility>

#include "core/data/vector2.hpp"
#include "entt/entity/fwd.hpp"
#include "entt/entity/registry.hpp"
#include "game/component/core/transform_component.hpp"
#include "game/component/core/interactive/click_action_component.hpp"
#include "game/component/core/rendering/rectangle_component.hpp"
#include "game/component/core/rendering/text_component.hpp"
#include "game/utility/color_palette.hpp"


Object::LabelButton::Data Object::LabelButton::Create(
    const SceneContext& context,
    Component::UI::Transform& transform,
    std::string&& display,
    const uint16_t id
) noexcept
{
    constexpr Nc::Vector2f DEFAULT_SIZE = Nc::Vector2f(420.0f, 68.0f);

    if (transform.size == Nc::Vector2f::Zero()) transform.size = DEFAULT_SIZE;

    const entt::entity button = Button::Create(context.registry, transform, id);
    const auto& entityTransform = context.registry.get<Component::UI::Transform>(button);
    const entt::entity label = Label::Create(context.registry, std::move(display), entityTransform);

    return {label, button};
}


entt::entity Object::LabelButton::Label::Create(
    entt::registry& registry,
    std::string&& label,
    const Component::UI::Transform& transform
) noexcept
{
    const entt::entity entity = registry.create();

    auto& labelTransform = registry.emplace<Component::UI::Transform>(entity, transform);
    ++labelTransform.index;

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


entt::entity Object::LabelButton::Button::Create(
    entt::registry& registry,
    Component::UI::Transform transform,
    uint16_t id
) noexcept
{
    const entt::entity entity = registry.create();

    registry.emplace<Component::UI::Transform>(entity, transform);
    registry.emplace<Component::Rectangle>(entity, Palette::BACKGROUND_COLOR);
    registry.emplace<Component::Action::Click>(entity, id);

    return entity;
}
