#include "game/component/core/rendering/lighting/light_source_component.hpp"
#include "game/component/core/transform_component.hpp"
#include "core/runtime/render_context.hpp"
#include "core/runtime/resource_store.hpp"
#include "game/state/game_state.hpp"
#include "entt/entity/fwd.hpp"
#include "entt/entity/registry.hpp"
#include "raylib.h"
#include "game/system/core/rendering/lighting/lighting_system.hpp"
#include "core/math/interpolation.hpp"
#include "core/data/vector2.hpp"



void LightingSystem::Initialize(LightingContext& context, ResourceStore& resourceStore)
{
    Shader& shader = resourceStore.GetShader("assets/lighting.fs");

    context.textureLocation = GetShaderLocation(shader, "texture0");
    context.lightSourceCount = GetShaderLocation(shader, "lightSourceCount");
    context.lightPositionLocation = GetShaderLocation(shader, "lightPosition[0]");
    context.lightColorLocation = GetShaderLocation(shader, "lightColor[0]");
    context.lightRangeLocation = GetShaderLocation(shader, "lightRange[0]");
    context.lightStrengthLocation = GetShaderLocation(shader, "lightStrength[0]");
}


void LightingSystem::Update(
    entt::registry& registry, 
    LightingContext& context, 
    Shader& lightShader, 
    GameState& gameState, 
    Nc::Vector2f cameraPosition, 
    float deltaTime
)
{
    int lightSourceCount = 0;
    auto view = registry.view<Component::Transform, Component::LightSource>();
    
    for (auto [entity, transform, source] : view.each())
    {
        if (transform.boundScene != gameState.currentScene) continue;
        ++lightSourceCount;
    }

    SetShaderValue(lightShader, context.lightSourceCount, &lightSourceCount, SHADER_UNIFORM_INT);

    int index = 0;
    for (auto [entity, transform, source] : view.each())
    {
        if (transform.boundScene != gameState.currentScene) continue;

        Nc::Vector2f position = transform.position - cameraPosition;
        SetShaderValue(lightShader, context.lightPositionLocation + index, &position, SHADER_UNIFORM_VEC3);
        SetShaderValue(lightShader, context.lightRangeLocation + index, &source.range, SHADER_UNIFORM_FLOAT);

        Vector4 color = source.color.ToFloat();
        float strength = source.strength + (source.strength * GetRandomValue(-8, 8) * 0.1f);
        source.currentStrength = Nc::Math::SmoothApproach(source.currentStrength, strength, deltaTime, 1.6f);
        
        SetShaderValue(lightShader, context.lightColorLocation + index, &color, SHADER_UNIFORM_VEC4);
        SetShaderValue(lightShader, context.lightStrengthLocation + index, &source.currentStrength, SHADER_UNIFORM_FLOAT);
        ++index;
    }
}