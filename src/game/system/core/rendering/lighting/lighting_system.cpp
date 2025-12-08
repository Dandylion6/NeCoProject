#include "core/data/color.hpp"
#include "core/data/vector2.hpp"
#include "core/math/interpolation.hpp"
#include "core/runtime/lighting_context.hpp"
#include "core/runtime/resource_store.hpp"
#include "entt/entity/fwd.hpp"
#include "entt/entity/registry.hpp"
#include "game/component/core/rendering/lighting/light_source_component.hpp"
#include "game/component/core/transform_component.hpp"
#include "game/state/game_state.hpp"
#include "game/system/core/rendering/lighting/lighting_system.hpp"
#include "raylib.h"



void LightingSystem::Initialize(Nc::LightingContext& context, Nc::ResourceStore& resourceStore)
{
    const Shader& shader = resourceStore.GetShader("assets/lighting.fs");

    context.textureLocation = GetShaderLocation(shader, "texture0");
    context.lightPointCount = GetShaderLocation(shader, "lightPointCount");
    context.lightPositionLocation = GetShaderLocation(shader, "lightPosition[0]");
    context.lightColorLocation = GetShaderLocation(shader, "lightColor[0]");
    context.lightRangeLocation = GetShaderLocation(shader, "lightRange[0]");
    context.lightStrengthLocation = GetShaderLocation(shader, "lightStrength[0]");
}


void LightingSystem::Update(
    entt::registry& registry, 
    Nc::LightingContext& context, 
    const Shader& lightShader, 
    GameState& gameState, 
    Nc::Vector2f cameraPosition, 
    float deltaTime
)
{
    int lightPointCount = 0;
    auto view = registry.view<Component::Transform, Component::Light::Point>();
    
    for (auto [entity, transform, source] : view.each())
    {
        if (transform.boundScene != gameState.currentScene) continue;
        ++lightPointCount;
    }

    SetShaderValue(lightShader, context.lightPointCount, &lightPointCount, SHADER_UNIFORM_INT);

    int index = 0;
    for (auto [entity, transform, source] : view.each())
    {
        if (transform.boundScene != gameState.currentScene) continue;

        Nc::Vector2f position = transform.position - cameraPosition;
        SetShaderValue(lightShader, context.lightPositionLocation + index, &position, SHADER_UNIFORM_VEC3);
        SetShaderValue(lightShader, context.lightRangeLocation + index, &source.range, SHADER_UNIFORM_FLOAT);

        Vector4 color = Nc::RGBa::FloatFrom(source.color);
        float strength = source.strength + (source.strength * GetRandomValue(-8, 8) * 0.1f);
        source.currentStrength = Nc::Math::SmoothApproach(source.currentStrength, strength, deltaTime, 1.6f);
        
        SetShaderValue(lightShader, context.lightColorLocation + index, &color, SHADER_UNIFORM_VEC4);
        SetShaderValue(lightShader, context.lightStrengthLocation + index, &source.currentStrength, SHADER_UNIFORM_FLOAT);
        ++index;
    }
}