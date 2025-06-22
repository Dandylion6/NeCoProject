#include "assemblers/entities/move_region_entity.h"
#include "assemblers/entities/scene_background_entity.h"
#include "assemblers/scenes/doorway_scene/doorway_scene.h"
#include "core/resource_store.h"
#include "core/scene.h"
#include "entt/entity/fwd.hpp"
#include "raylib.h"
#include "utility/vector2.h" 


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