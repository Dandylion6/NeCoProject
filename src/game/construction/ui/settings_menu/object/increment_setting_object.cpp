#include "game/construction/ui/settings_menu/object/increment_setting_object.hpp"

#include <iomanip>
#include <string>
#include <utility>

#include "raylib.h"
#include "core/data/vector2.hpp"
#include "core/runtime/resource_store.hpp"
#include "entt/entity/fwd.hpp"
#include "entt/entity/registry.hpp"
#include "game/component/core/transform_component.hpp"
#include "game/component/core/interactive/click_action_component.hpp"
#include "game/component/core/rendering/text_component.hpp"
#include "game/component/ui/increment_button_component.hpp"
#include "game/component/ui/increment_component.hpp"
#include "game/contexts/scene_context.hpp"
#include "game/utility/color_palette.hpp"


Object::IncrementSetting::Data Object::IncrementSetting::Create(
    const SceneContext& context,
    std::string&& display,
    const Component::UI::Increment& increment,
    const Nc::Vector2f position
) noexcept
{
    const Font& font = context.store.GetFont(Nc::Font::WDXL, Nc::Font::Size::Medium);
    const float fontSize = Nc::Font::SizeToFloat(Nc::Font::Size::Medium);

    auto size = Nc::Vector2f(MeasureTextEx(font, display.c_str(), fontSize, 0.0f));
    size.y = 0.0f;

    const entt::entity label = Label::Create(context.registry, std::move(display), position);

    const Nc::Vector2f displayOffset = size + Nc::Vector2f::Right(28.0f);
    const entt::entity valueDisplay = ValueDisplay::Create(context.registry, increment, position, displayOffset);

    const auto incrementContext = IncrementContext(context.registry, position, size, valueDisplay);

    const entt::entity decreaseButton = DecreaseButton::Create(incrementContext);
    const entt::entity increaseButton = IncreaseButton::Create(incrementContext);

    return {label, valueDisplay, decreaseButton, increaseButton};
}


entt::entity Object::IncrementSetting::Label::Create(
    entt::registry& registry,
    std::string&& display,
    Nc::Vector2f position
) noexcept
{
    const entt::entity entity = registry.create();

    registry.emplace<Component::UI::Transform>(entity, position, Nc::Vector2f::Zero(), 2);
    registry.emplace<Component::Text>(
        entity,
        std::move(display),
        Palette::RADAR_COLOR,
        Nc::Font::WDXL,
        Nc::Font::Size::Medium,
        Alignment::Left
    );

    return entity;
}


entt::entity Object::IncrementSetting::ValueDisplay::Create(
    entt::registry& registry,
    Component::UI::Increment increment,
    Nc::Vector2f position,
    Nc::Vector2f offset
) noexcept
{
    constexpr Nc::Vector2f ORIGIN = Nc::Vector2f::Scale(0.5f);

    const entt::entity entity = registry.create();

    registry.emplace<Component::UI::Transform>(entity, position, ORIGIN, Nc::Vector2f::Zero(), offset, 2);
    registry.emplace<Component::Text>(entity, "", Palette::RADAR_COLOR, Nc::Font::WDXL, Nc::Font::Size::Medium);
    registry.emplace<Component::UI::Increment>(entity, increment);

    return entity;
}


entt::entity Object::IncrementSetting::IncreaseButton::Create(const IncrementContext& context) noexcept
{
    constexpr Nc::Vector2f ORIGIN = Nc::Vector2f::Scale(0.5f);
    constexpr Nc::Vector2f SIZE = Nc::Vector2f::Scale(32.0f);

    const entt::entity entity = context.registry.create();

    const Nc::Vector2f offset = context.offset + Nc::Vector2f(72.0f, 2.0f);
    context.registry.emplace<Component::UI::Transform>(entity, context.position, ORIGIN, SIZE, offset, 2);
    context.registry.emplace<Component::Text>(entity, "+", Nc::RGBa(RAYWHITE), Nc::Font::WDXL, Nc::Font::Size::Medium);

    context.registry.emplace<Component::Action::Click>(entity);
    context.registry.emplace<Component::UI::IncrementButton>(
        entity,
        context.increment,
        Component::UI::IncrementButton::Increase
    );
    return entity;
}


entt::entity Object::IncrementSetting::DecreaseButton::Create(const IncrementContext& context) noexcept
{
    constexpr Nc::Vector2f ORIGIN = Nc::Vector2f::Scale(0.5f);
    constexpr Nc::Vector2f SIZE = Nc::Vector2f::Scale(24.0f);

    const entt::entity entity = context.registry.create();

    const Nc::Vector2f offset = context.offset + Nc::Vector2f::Right(98.0f);
    context.registry.emplace<Component::UI::Transform>(entity, context.position, ORIGIN, SIZE, offset, 2);
    context.registry.emplace<Component::Text>(entity, "-", Nc::RGBa(RAYWHITE), Nc::Font::WDXL, Nc::Font::Size::Large);

    context.registry.emplace<Component::Action::Click>(entity);
    context.registry.emplace<Component::UI::IncrementButton>(
        entity,
        context.increment,
        Component::UI::IncrementButton::Decrease
    );
    return entity;
}
