#include "game/system/core/rendering/rendering_system.hpp"

#include <algorithm>
#include <vector>

#include "core/data/vector2.hpp"
#include "core/runtime/render_context.hpp"
#include "entt/entity/fwd.hpp"
#include "entt/entity/registry.hpp"
#include "game/component/core/transform_component.hpp"
#include "game/component/core/rendering/rectangle_component.hpp"
#include "game/component/core/rendering/sprite_component.hpp"
#include "game/component/core/rendering/text_component.hpp"
#include "game/state/game_state.hpp"
#include "game/state/scene.hpp"
#include "game/system/core/rendering/rectangle_render_system.hpp"
#include "game/system/core/rendering/sprite_render_system.hpp"
#include "game/system/core/rendering/text_render_system.hpp"


void RenderingSystem::DrawScreen(
    const Shader& lightShader,
    entt::registry& registry,
    const GameState& gameState,
    const Nc::Vector2f cameraPosition
)
{
    std::vector<Renderable> entities{ };

    const auto view = registry.view<Component::Transform>();
    for (const auto& [entity, transform] : view.each())
    {
        RenderType type = GetRenderType(registry, entity);
        if (type == RenderType::Invalid) continue;
        if (!ShouldRender(transform, gameState.currentScene)) continue;
        entities.emplace_back(entity, type, transform.index);
    }

    std::ranges::sort(entities, SortComparison);

    for (const Renderable renderable : entities)
    {
        switch (renderable.type)
        {
        case RenderType::Sprite:
            SpriteRenderSystem::DrawScreen(lightShader, renderable.entity, registry, cameraPosition);
            break;
        case RenderType::Rectangle:
            RectangleRenderSystem::DrawScreen(renderable.entity, registry, cameraPosition);
            break;
        case RenderType::Text:
        default:
            break;
        }
    }
}


void RenderingSystem::DrawUi(
    entt::registry& registry,
    Nc::ResourceStore& resourceStore,
    const Nc::RenderContext& renderContext
)
{
    std::vector<Renderable> entities{ };

    const auto viewUi = registry.view<Component::UI::Transform>();
    for (const auto& [entity, transform] : viewUi.each())
    {
        RenderType type = GetRenderType(registry, entity);
        if (type == RenderType::Invalid) continue;
        if (!ShouldRender(transform)) continue;
        entities.emplace_back(entity, type, transform.index);
    }

    std::ranges::sort(entities, SortComparison);

    for (const Renderable renderable : entities)
    {
        switch (renderable.type)
        {
        case RenderType::Sprite:
            SpriteRenderSystem::DrawUi(renderable.entity, registry, Nc::Vector2f(renderContext.windowSize));
            break;
        case RenderType::Rectangle:
            RectangleRenderSystem::DrawUi(renderable.entity, registry, Nc::Vector2f(renderContext.windowSize));
            break;
        case RenderType::Text:
            TextRenderSystem::DrawUi(
                renderable.entity,
                registry,
                resourceStore,
                Nc::Vector2f(renderContext.windowSize)
            );
            break;
        default:
            break;
        }
    }
}


RenderingSystem::RenderType RenderingSystem::GetRenderType(const entt::registry& registry, const entt::entity entity)
{
    if (registry.all_of<Component::Sprite>(entity)) return RenderType::Sprite;
    if (registry.all_of<Component::Rectangle>(entity)) return RenderType::Rectangle;
    if (registry.all_of<Component::Text>(entity)) return RenderType::Text;
    return RenderType::Invalid;
}


bool RenderingSystem::ShouldRender(const Component::Transform& transform, const Scene currentScene)
{
    if (transform.boundScene == currentScene) return true;
    if (transform.boundScene == NullScene) return true;
    return false;
}


bool RenderingSystem::ShouldRender(const Component::UI::Transform& transform)
{
    return transform.isVisible;
}


bool RenderingSystem::SortComparison(const Renderable a, const Renderable b)
{
    return a.index < b.index;
}
