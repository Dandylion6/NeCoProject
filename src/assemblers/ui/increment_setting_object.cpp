#include "assemblers/ui/increment_setting_object.hpp"
#include "components/core/button_action_component.hpp"
#include "components/core/rendering/rectangle_component.hpp"
#include "components/core/rendering/text_component.hpp"
#include "components/core/transform_component.hpp"
#include "components/ui/increment_component.hpp"
#include "components/ui/settings_tag.hpp"
#include "core/resource_store.hpp"
#include "entt/entity/fwd.hpp"
#include "entt/entity/registry.hpp"
#include "raylib.h"
#include "utility/color_palette.hpp"
#include "utility/vector2.hpp"
#include <functional>
#include <iomanip>
#include <ios>
#include <sstream>
#include <string>
#include <utility>


namespace Construct
{
    void LabelEntity(
        Nc::Vector2f position,
        std::string&& display, 
        entt::registry& registry, 
        ResourceStore& resourceStore
    )
    {
        const entt::entity entity = registry.create();

        registry.emplace<Tag::Settings>(entity);
        registry.emplace<Component::UiTransform>(entity, position, Nc::Vector2f::Zero(), Nc::Vector2f::Zero(), Nc::Vector2f::Zero(), 2);
        registry.emplace<Component::Text>(entity, std::move(display), RADAR_COLOR, WDXL, FontSize::Medium);
    };


    const entt::entity ValueDisplayEntity(
        Nc::Vector2f position,
        Nc::Vector2f offset,
        entt::registry& registry,
        ResourceStore& resourceStore
    )
    {
        const entt::entity entity = registry.create();

        registry.emplace<Tag::Settings>(entity);
        registry.emplace<Component::UiTransform>(entity, position, Nc::Vector2f::Scale(0.5f), Nc::Vector2f::Zero(), offset, 2);
        registry.emplace<Component::Text>(entity, "", RADAR_COLOR, WDXL, FontSize::Medium);

        return entity;
    };


    void IncreaseButton(
        Nc::Vector2f position,
        Nc::Vector2f offset,
        Component::UiIncrement& increment,
        Component::Text& valueDisplay,
        entt::registry& registry,
        ResourceStore& resourceStore
    )
    {
        const entt::entity entity = registry.create();

        registry.emplace<Tag::Settings>(entity);

        registry.emplace<Component::UiTransform>(entity, position, Nc::Vector2f::Scale(0.5f), Nc::Vector2f::Scale(32.0f), offset + Nc::Vector2f::Up(2.0f), 2);
        registry.emplace<Component::Text>(entity, "+", RAYWHITE, WDXL, FontSize::Medium);

        std::function<void()> onClick = [increment = increment, &valueDisplay]()
        {
            float newValue = increment.onIncrement(increment.increment);

            std::stringstream stringStream;
            stringStream << std::fixed << std::setprecision(increment.decimals) << newValue;
            
            valueDisplay.text = stringStream.str();
        };

        onClick();
        registry.emplace<Component::ButtonAction>(entity, std::move(onClick));
    };


    void DecreaseButton(
        Nc::Vector2f position,
        Nc::Vector2f offset,
        Component::UiIncrement& increment, 
        Component::Text& valueDisplay,
        entt::registry& registry,
        ResourceStore& resourceStore
    )
    {
        const entt::entity entity = registry.create();

        registry.emplace<Tag::Settings>(entity);

        registry.emplace<Component::UiTransform>(entity, position, Nc::Vector2f::Scale(0.5f), Nc::Vector2f::Scale(24.0f), offset, 2);
        registry.emplace<Component::Text>(entity, "-", RAYWHITE, WDXL, FontSize::Large);

        std::function<void()> onClick = [increment = increment, &valueDisplay]()
        {
            float newValue = increment.onIncrement(-increment.increment);

            std::stringstream stringStream;
            stringStream << std::fixed << std::setprecision(increment.decimals) << newValue;
            
            valueDisplay.text = stringStream.str();
        };

        onClick();
        registry.emplace<Component::ButtonAction>(entity, std::move(onClick));
    };
}


void Construct::IncrementSettingObject(
    Nc::Vector2f position,
    std::string&& display, 
    Component::UiIncrement&& increment,
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
    size.x *= 0.5f;
    
    Construct::LabelEntity(position, std::move(display), registry, resourceStore);

    const entt::entity entity = ValueDisplayEntity(position, size + Nc::Vector2f::Right(28.0f), registry, resourceStore);
    Component::Text& valueText = registry.get<Component::Text>(entity); 

    Construct::DecreaseButton(position, size + Nc::Vector2f::Right(72.0f), increment, valueText, registry, resourceStore);
    Construct::IncreaseButton(position, size + Nc::Vector2f::Right(98.0f), increment, valueText, registry, resourceStore);

    float currentValue = increment.onIncrement(0.0f);

    std::stringstream stringStream;
    stringStream << std::fixed << std::setprecision(increment.decimals) << currentValue;
            
    valueText.text = stringStream.str();
};