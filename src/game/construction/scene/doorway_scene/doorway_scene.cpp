#include "core/data/color.hpp"
#include "core/data/vector2.hpp" 
#include "core/runtime/render_context.hpp"
#include "core/runtime/resource_store.hpp"
#include "entt/entity/fwd.hpp"
#include "game/construction/scene/doorway_scene/doorway_scene.hpp"
#include "game/construction/shared/entity/environment/light_source_entity.hpp"
#include "game/construction/shared/entity/scene/move_region_entity.hpp"
#include "game/construction/shared/entity/scene/scene_background_entity.hpp"
#include "game/state/scene.hpp"
#include "raylib.h"
#include <utility>


void Structure::DoorwayScene::Build(
	entt::registry& registry, 
	Nc::ResourceStore& resourceStore, 
	Nc::RenderContext& renderContext,
	GameState& gameState
) noexcept
{
	constexpr Nc::Hex LIGHT_COLOR = 0xfee8c8ff;
	
	Texture2D backgroundText = resourceStore.GetTexture("assets/environment/backgrounds/doorway.png");
	Entity::SceneBackground::Create(std::move(backgroundText), registry, Doorway);
	Entity::MoveRegion::Create(registry, resourceStore, gameState, Left, Doorway, CommsRoom, 0.4f);

	Nc::Vector2f windowSize = Nc::Vector2f(renderContext.windowSize);
	Nc::Vector2f lightPosition = windowSize * Nc::Vector2f(0.5f, 1.3f);
	float lightRadius = 940.0f * renderContext.renderScale;
	
	Entity::LightPoint::Create(registry, Doorway, lightPosition, LIGHT_COLOR, 1.6f, lightRadius);
}