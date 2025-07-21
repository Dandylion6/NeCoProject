#include "components/core/rendering/sprite_component.hpp"
#include "components/core/transform_component.hpp"
#include "core/render_context.hpp"
#include "core/scene.hpp"
#include "entt/entity/fwd.hpp"
#include "entt/entity/registry.hpp"
#include "raylib.h"
#include "systems/core/rendering_system.hpp"


void RenderingSystem::Draw(
    entt::registry& registry, RenderContext& renderContext, Scene currentScene
)
{
    BeginTextureMode(renderContext.renderTexture);

    auto screenView = registry.view<const Component::Transform>();

    EndTextureMode();
}


RenderingSystem::RenderType RenderingSystem::GetRenderType(
    entt::registry& registry, const entt::entity entity, Scene currentScene
)
{
    const Component::Transform* transform = registry.try_get<const Component::Transform>(entity);
    if (transform == nullptr) return Ignore;
    if (transform->boundScene != currentScene && transform->boundScene != NullScene) return Ignore;

    if (registry.all_of<Component::Sprite>(entity)) return Sprite;
    return Ignore;
}
