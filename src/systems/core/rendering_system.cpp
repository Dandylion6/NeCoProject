#include "components/core/rendering/rectangle_component.hpp"
#include "components/core/rendering/sprite_component.hpp"
#include "components/core/rendering/text_component.hpp"
#include "components/core/transform_component.hpp"
#include "core/game_state.hpp"
#include "core/render_context.hpp"
#include "core/scene.hpp"
#include "entt/entity/entity.hpp"
#include "entt/entity/fwd.hpp"
#include "entt/entity/registry.hpp"
#include "systems/core/rendering_system.hpp"
#include "systems/core/rendering/rectangle_render_system.hpp"
#include "systems/core/rendering/sprite_render_system.hpp"
#include "systems/core/rendering/text_render_system.hpp"
#include "systems/object/comms/radar/radar_render_system.hpp"
#include "raylib.h"
#include "utility/vector2.hpp"
#include <algorithm>
#include <functional>


void RenderingSystem::Draw(
    entt::registry& registry, RenderContext& renderContext, GameState& gameState
)
{
    struct Renderable
    {
        entt::entity entity = entt::null;
        RenderType type = RenderType::Invalid;
        SortIndex index = 0;
    };

    std::function<bool(Renderable, Renderable)> comparison = [](Renderable a, Renderable b)
    {
        return a.index < b.index;
    };

    std::vector<Renderable> entities { };

    Nc::Vector2f cameraPosition = Nc::Vector2f::Zero();
	cameraPosition.x += std::cosf(gameState.time * 1.4f) * 5.0f;
	cameraPosition.y += std::sinf((gameState.time * 2.8f) - 0.3f) * 4.0f;

    BeginTextureMode(renderContext.renderTexture);

    auto view = registry.view<Component::Transform>();
    for (auto [entity, transform] : view.each())
    {
        RenderType type = GetRenderType(registry, entity);
        if (type == RenderType::Invalid) continue;
        if (!ShouldRender(transform, gameState.currentScene)) continue;
        entities.emplace_back(entity, type, transform.index);
    }

    std::sort(entities.begin(), entities.end(), comparison);

    for (Renderable renderable : entities)
    {
        switch (renderable.type) 
        {
        case RenderType::Sprite:
            SpriteRenderSystem::DrawScreen(renderable.entity, registry, cameraPosition);
            break;
        case RenderType::Rectangle:
            RectangleRenderSystem::DrawScreen(renderable.entity, registry, cameraPosition);
            break;
        case RenderType::Text:
            break;
        default:
            break;
        }
    }

    RadarRenderSystem::DrawRadar(
		registry, renderContext.radarRenderTexture, cameraPosition, gameState.currentScene
	);

    EndTextureMode();

    BeginDrawing();
	ClearBackground(RenderContext::BACKGROUND_COLOR);

	Nc::Vector2f displaySize = RenderContext::DISPLAY_SIZE;
	Rectangle source { 0, 0, displaySize.x, -displaySize.y };

	DrawTexturePro(
		renderContext.renderTexture.texture,
		source,
		renderContext.renderRectangle,
		Nc::Vector2f::Zero(),
		0.0f,
		WHITE
	);

    entities.clear();

    auto viewUi = registry.view<Component::UiTransform>();
    for (auto [entity, transform] : viewUi.each())
    {
        RenderType type = GetRenderType(registry, entity);
        if (type == RenderType::Invalid) continue;
        if (!ShouldRender(transform)) continue;
        entities.emplace_back(entity, type, transform.index);
    }

    std::sort(entities.begin(), entities.end(), comparison);

    for (Renderable renderable : entities)
    {
        switch (renderable.type) 
        {
        case RenderType::Sprite:
            SpriteRenderSystem::DrawUi(renderable.entity, registry, renderContext.windowSize);
            break;
        case RenderType::Rectangle:
            RectangleRenderSystem::DrawUi(renderable.entity, registry, renderContext.windowSize);
            break;
        case RenderType::Text:
            break;
        default:
            break;
        }
    }

    EndDrawing();
}



RenderingSystem::RenderType RenderingSystem::GetRenderType(
    entt::registry& registry, const entt::entity entity
)
{
    if (registry.all_of<Component::Sprite>(entity)) return RenderType::Sprite;
    if (registry.all_of<Component::Rectangle>(entity)) return RenderType::Rectangle;
    if (registry.all_of<Component::Text>(entity)) return RenderType::Text;
    return RenderType::Invalid;
}


bool RenderingSystem::ShouldRender(
	const Component::Transform& transform, Scene currentScene
)
{
    if (transform.boundScene == currentScene) return true;
    if (transform.boundScene == NullScene) return true;
    return false;
}


bool RenderingSystem::ShouldRender(
    const Component::UiTransform& transform
)
{
    return transform.isVisible;
}