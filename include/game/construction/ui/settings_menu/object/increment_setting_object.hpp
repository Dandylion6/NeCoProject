#pragma once
#include "game/component/core/interactive/button_action_component.hpp"
#include "game/component/core/rendering/text_component.hpp"
#include "game/component/core/transform_component.hpp"
#include "game/component/core/interactive/button_action_component.hpp"
#include "game/component/core/rendering/text_component.hpp"
#include "game/component/core/transform_component.hpp"
#include "game/component/ui/increment_component.hpp"
#include "core/runtime/resource_store.hpp"
#include "entt/entity/fwd.hpp"
#include "entt/entity/registry.hpp"
#include "raylib.h"
#include "game/utility/color_palette.hpp"
#include "core/runtime/resource_store.hpp"
#include "entt/entity/fwd.hpp"
#include "entt/entity/registry.hpp"
#include "raylib.h"
#include "game/utility/color_palette.hpp"
#include "core/data/vector2.hpp"
#include <functional>
#include <iomanip>
#include <ios>
#include <sstream>
#include <string>
#include <utility>


namespace Construct
{   
    inline const entt::entity LabelEntity(
        Nc::Vector2f position,
        std::string&& display, 
        entt::registry& registry
    )
    {
        const entt::entity entity = registry.create();

        registry.emplace<Component::UI::Transform>(entity, position, Nc::Vector2f::Zero(), Nc::Vector2f::Zero(), Nc::Vector2f::Zero(), 2);
        registry.emplace<Component::Text>(entity, std::move(display), Palette::RADAR_COLOR, WDXL, FontSize::Medium, Alignment::Left);

        return entity;
    };


    inline const entt::entity ValueDisplayEntity(
        Nc::Vector2f position,
        Nc::Vector2f offset,
        Component::UI::Increment&& increment,
        entt::registry& registry
    )
    {
        const entt::entity entity = registry.create();

        registry.emplace<Component::UI::Transform>(entity, position, Nc::Vector2f::Scale(0.5f), Nc::Vector2f::Zero(), offset, 2);
        registry.emplace<Component::Text>(entity, "", Palette::RADAR_COLOR, WDXL, FontSize::Medium);
        registry.emplace<Component::UI::Increment>(entity, increment);

        return entity;
    };


    inline const entt::entity IncreaseButton(
        Nc::Vector2f position,
        Nc::Vector2f offset,
        Component::UI::Increment& increment,
        Component::Text& valueDisplay,
        entt::registry& registry
    )
    {
        const entt::entity entity = registry.create();

        registry.emplace<Component::UI::Transform>(entity, position, Nc::Vector2f::Scale(0.5f), Nc::Vector2f::Scale(32.0f), offset + Nc::Vector2f::Up(2.0f), 2);
        registry.emplace<Component::Text>(entity, "+", RAYWHITE, WDXL, FontSize::Medium);

        std::function<void()> onClick = [increment = increment, &valueDisplay]()
        {
            if (increment.value == nullptr) return;
            *increment.value = (*increment.value) + increment.increment;
        };

        registry.emplace<Component::ButtonAction>(entity, std::move(onClick));
        return entity;
    };


    inline const entt::entity DecreaseButton(
        Nc::Vector2f position,
        Nc::Vector2f offset,
        Component::UI::Increment& increment, 
        Component::Text& valueDisplay,
        entt::registry& registry
    )
    {
        const entt::entity entity = registry.create();

        registry.emplace<Component::UI::Transform>(entity, position, Nc::Vector2f::Scale(0.5f), Nc::Vector2f::Scale(24.0f), offset, 2);
        registry.emplace<Component::Text>(entity, "-", RAYWHITE, WDXL, FontSize::Large);

        std::function<void()> onClick = [increment = increment, &valueDisplay]()
        {
            if (increment.value == nullptr) return;
            *increment.value = (*increment.value) - increment.increment;
        };

        registry.emplace<Component::ButtonAction>(entity, std::move(onClick));
        return entity;
    };


    template<class... T>
    inline void IncrementSettingObject(
        Nc::Vector2f position,
        std::string&& display,
        Component::UI::Increment&& increment,
        entt::registry& registry, 
        ResourceStore& resourceStore
    )
    {
        Nc::Vector2f size = MeasureTextEx(
            resourceStore.GetFont(WDXL, static_cast<uint8_t>(FontSize::Medium)),
            display.c_str(), 
            static_cast<float>(FontSize::Medium), 
            0.0f
        );
        size.y = 0.0f;
        
        const entt::entity label = Construct::LabelEntity(position, std::move(display), registry);

        const entt::entity valueDisplay = ValueDisplayEntity(position, size + Nc::Vector2f::Right(28.0f), std::move(increment), registry);
        Component::Text& valueText = registry.get<Component::Text>(valueDisplay); 

        const entt::entity decrease = Construct::DecreaseButton(position, size + Nc::Vector2f::Right(72.0f), increment, valueText, registry);
        const entt::entity increase = Construct::IncreaseButton(position, size + Nc::Vector2f::Right(98.0f), increment, valueText, registry);

        (registry.emplace<T>(label), ...);
        (registry.emplace<T>(valueDisplay), ...);
        (registry.emplace<T>(decrease), ...);
        (registry.emplace<T>(increase), ...);
    };
}