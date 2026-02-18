#include "game/system/core/rendering/lighting/lighting_system.hpp"

#include "raylib.h"
#include "core/data/color.hpp"
#include "core/data/vector2.hpp"
#include "core/math/random.hpp"
#include "core/math/vector_math.hpp"
#include "core/runtime/lighting_context.hpp"
#include "core/runtime/render_context.hpp"
#include "core/runtime/resource_store.hpp"
#include "entt/entity/fwd.hpp"
#include "entt/entity/registry.hpp"
#include "game/component/core/transform_component.hpp"
#include "game/component/core/rendering/lighting/light_source_component.hpp"
#include "game/contexts/system_context.hpp"
#include "game/state/game_state.hpp"


void System::Render::Lighting::Initialize(Nc::LightingContext& context, Nc::ResourceStore& resourceStore)
{
    const Shader& shader = resourceStore.GetShader("assets/lighting.fs");

    context.lightPointCount = GetShaderLocation(shader, "lightPointCount");
    context.lightPositionLocation = GetShaderLocation(shader, "lightPosition[0]");
    context.lightColorLocation = GetShaderLocation(shader, "lightColor[0]");
    context.lightRangeLocation = GetShaderLocation(shader, "lightRange[0]");
    context.lightStrengthLocation = GetShaderLocation(shader, "lightStrength[0]");
}


const Shader& System::Render::Lighting::Update(const SystemContext& systemContext, const Nc::RenderContext& context)
{
    const Shader& shader = systemContext.store.GetShader("assets/lighting.fs");

    int index = 0;
    const auto view = systemContext.registry.view<Component::Transform, Component::Light::Point>();
    for (auto [entity, transform, source] : view.each())
    {
        if (transform.boundScene != systemContext.game.currentScene) continue;

        const Nc::Vector2f position = transform.position - context.cameraPosition;
        Vector3 position3D = {position.x, position.y, source.zPosition};

        SetShaderValue(shader, context.lightingContext.lightPositionLocation + index, &position3D, SHADER_UNIFORM_VEC3);
        SetShaderValue(shader, context.lightingContext.lightRangeLocation + index, &source.range, SHADER_UNIFORM_FLOAT);

        Vector4 color = Nc::RGBa::FloatFrom(source.color);
        SetShaderValue(shader, context.lightingContext.lightColorLocation + index, &color, SHADER_UNIFORM_VEC4);
        SetShaderValue(shader, context.lightingContext.lightStrengthLocation + index, &source.currentStrength, SHADER_UNIFORM_FLOAT);
        ++index;
    }

    SetShaderValue(shader, context.lightingContext.lightPointCount, &index, SHADER_UNIFORM_INT);
    return shader;
}