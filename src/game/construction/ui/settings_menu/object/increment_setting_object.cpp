#include "game/construction/ui/settings_menu/object/increment_setting_object.hpp"
#include "core/data/vector2.hpp"
#include "core/runtime/resource_store.hpp"
#include "entt/entity/fwd.hpp"
#include "entt/entity/registry.hpp"
#include "game/component/core/interactive/click_action_component.hpp"
#include "game/component/core/rendering/text_component.hpp"
#include "game/component/core/transform_component.hpp"
#include "game/component/ui/increment_component.hpp"
#include "game/utility/color_palette.hpp"
#include "raylib.h"
#include <functional>
#include <iomanip>
#include <ios>
#include <sstream>
#include <string>
#include <utility>


const Object::IncrementSetting::Data Object::IncrementSetting::Create(
    entt::registry& registry, 
    Nc::ResourceStore& resourceStore, 
    std::string&& display, 
    Component::UI::Increment&& increment,
    Nc::Vector2f position
) noexcept
{
    Nc::Vector2f size = MeasureTextEx(
        resourceStore.GetFont(Nc::Font::WDXL, Nc::Font::Size::Medium),
        display.c_str(), 
        static_cast<float>(Nc::Font::Size::Medium), 
        0.0f
    );
    size.y = 0.0f;
    
    const entt::entity label = Label::Create(registry, std::move(display), position);

    Nc::Vector2f displayOffset = size + Nc::Vector2f::Right(28.0f);
    const entt::entity valueDisplay = ValueDisplay::Create(registry, std::move(increment), position, displayOffset);
    Component::Text& valueText = registry.get<Component::Text>(valueDisplay); 

    Nc::Vector2f increaseOffset = size + Nc::Vector2f::Right(72.0f);
    const entt::entity decreaseButton = DecreaseButton::Create(registry, increment, valueText, position, increaseOffset);

    Nc::Vector2f decreaseOffset = size + Nc::Vector2f::Right(98.0f);
    const entt::entity increaseButton = IncreaseButton::Create(registry, increment, valueText, position, decreaseOffset);

    return { label, valueDisplay, decreaseButton, increaseButton };

}


const entt::entity Object::IncrementSetting::Label::Create(
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


const entt::entity Object::IncrementSetting::ValueDisplay::Create(
    entt::registry& registry, 
    Component::UI::Increment&& increment, 
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


const entt::entity Object::IncrementSetting::IncreaseButton::Create(
    entt::registry& registry, 
    Component::UI::Increment& increment, 
    Component::Text& valueDisplay, 
    Nc::Vector2f position, 
    Nc::Vector2f offset
) noexcept
{
    constexpr Nc::Vector2f ORIGIN = Nc::Vector2f::Scale(0.5f);
    constexpr Nc::Vector2f SIZE = Nc::Vector2f::Scale(32.0f);
    
    const entt::entity entity = registry.create();

    offset += Nc::Vector2f::Up(2.0f);
    registry.emplace<Component::UI::Transform>(entity, position, ORIGIN, SIZE, offset, 2);
    registry.emplace<Component::Text>(entity, "+", RAYWHITE, Nc::Font::WDXL, Nc::Font::Size::Medium);

    std::function<void()> onClick = [increment = increment, &valueDisplay]()
    {
        if (increment.value == nullptr) return;
        *increment.value = (*increment.value) + increment.increment;
    };

    registry.emplace<Component::Action::Click>(entity, std::move(onClick));
    return entity;
}


const entt::entity Object::IncrementSetting::DecreaseButton::Create(
    entt::registry& registry,
    Component::UI::Increment& increment, 
    Component::Text& valueDisplay, 
    Nc::Vector2f position, 
    Nc::Vector2f offset 
) noexcept
{
    constexpr Nc::Vector2f ORIGIN = Nc::Vector2f::Scale(0.5f);
    constexpr Nc::Vector2f SIZE = Nc::Vector2f::Scale(24.0f);

    const entt::entity entity = registry.create();

    registry.emplace<Component::UI::Transform>(entity, position, ORIGIN, SIZE, offset, 2);
    registry.emplace<Component::Text>(entity, "-", RAYWHITE, Nc::Font::WDXL, Nc::Font::Size::Large);

    std::function<void()> onClick = [increment = increment, &valueDisplay]()
    {
        if (increment.value == nullptr) return;
        *increment.value = (*increment.value) - increment.increment;
    };

    registry.emplace<Component::Action::Click>(entity, std::move(onClick));
    return entity;
}
