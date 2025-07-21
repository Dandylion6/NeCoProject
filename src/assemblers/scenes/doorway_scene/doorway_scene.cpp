#include "assemblers/entities/move_region_entity.hpp"
#include "assemblers/entities/scene_background_entity.hpp"
#include "assemblers/scenes/doorway_scene/doorway_scene.hpp"
#include "core/resource_store.hpp"
#include "core/scene.hpp"
#include "entt/entity/fwd.hpp"
#include "raylib.h"
#include "utility/vector2.hpp" 


void DoorwayScene::Build(
	entt::registry& registry, 
	GameState& gameState,
	ResourceStore& resourceStore
)
{
	Construct::SceneBackgroundEntity(
		LoadTexture("assets/environment/backgrounds/doorway.png"), registry, Doorway
	);

	Construct::MoveRegionEntity(
		registry, gameState, resourceStore, Left, Doorway, CommsRoom, 0.4f
	);
}