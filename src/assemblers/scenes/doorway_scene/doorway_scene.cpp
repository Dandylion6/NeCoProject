#include "assemblers/entities/light_source_entity.hpp"
#include "assemblers/entities/move_region_entity.hpp"
#include "assemblers/entities/scene_background_entity.hpp"
#include "assemblers/scenes/doorway_scene/doorway_scene.hpp"
#include "assemblers/scenes/doorway_scene/radar_breaker_object.hpp"
#include "core/context/render_context.hpp"
#include "core/resource_store.hpp"
#include "core/scene.hpp"
#include "entt/entity/fwd.hpp"
#include "raylib.h"
#include "utility/color.hpp"
#include "utility/vector2.hpp" 
#include <utility>


void DoorwayScene::Build(
	entt::registry& registry, GameState& gameState, ResourceStore& resourceStore, RenderContext& renderContext
)
{
	Construct::RadarBreakerObject(registry, resourceStore);

	Texture2D backgroundText = resourceStore.GetTexture("assets/environment/backgrounds/doorway.png");
	Construct::SceneBackgroundEntity(std::move(backgroundText), registry, Doorway);
	Construct::MoveRegionEntity(registry, gameState, resourceStore, Left, Doorway, CommsRoom, 0.4f);

	constexpr Nc::Hex LIGHT_COLOR = 0xfee8c8ff;

	Nc::Vector2f windowSize = renderContext.windowSize;
	Nc::Vector2f lightPosition = windowSize * Nc::Vector2f(0.5f, 1.3f);
	float lightRadius = 940.0f * renderContext.renderScale;
	Construct::LightSourceEntity(registry, lightPosition, Doorway, LIGHT_COLOR, 1.6f, lightRadius);
}