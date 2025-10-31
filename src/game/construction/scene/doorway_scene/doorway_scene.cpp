#include "game/construction/shared/entity/environment/light_source_entity.hpp"
#include "game/construction/shared/entity/scene/move_region_entity.hpp"
#include "game/construction/shared/entity/scene/scene_background_entity.hpp"
#include "game/construction/scene/doorway_scene/doorway_scene.hpp"
#include "core/runtime/render_context.hpp"
#include "core/runtime/resource_store.hpp"
#include "game/state/scene.hpp"
#include "entt/entity/fwd.hpp"
#include "raylib.h"
#include "core/data/color.hpp"
#include "core/data/vector2.hpp" 
#include <utility>


void DoorwayScene::Build(
	entt::registry& registry, GameState& gameState, ResourceStore& resourceStore, RenderContext& renderContext
)
{
	Texture2D backgroundText = resourceStore.GetTexture("assets/environment/backgrounds/doorway.png");
	Construct::SceneBackgroundEntity(std::move(backgroundText), registry, Doorway);
	Construct::MoveRegionEntity(registry, gameState, resourceStore, Left, Doorway, CommsRoom, 0.4f);

	constexpr Nc::Hex LIGHT_COLOR = 0xfee8c8ff;

	Nc::Vector2f windowSize = renderContext.windowSize;
	Nc::Vector2f lightPosition = windowSize * Nc::Vector2f(0.5f, 1.3f);
	float lightRadius = 940.0f * renderContext.renderScale;
	Construct::LightSourceEntity(registry, lightPosition, Doorway, LIGHT_COLOR, 1.6f, lightRadius);
}