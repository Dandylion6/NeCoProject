#include "core/resource_store.h"
#include "core/scene.h"
#include "entt/entity/fwd.hpp"
#include "modules/scenes/doorway_scene.h"
#include "raylib.h"
#include "utility/entity/move_entities.h"
#include "utility/entity/scene_entities.h"
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