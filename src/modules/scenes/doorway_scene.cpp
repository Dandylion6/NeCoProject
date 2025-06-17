#include "core/ecs_context.h"
#include "core/resource_store.h"
#include "core/scene.h"
#include "modules/scenes/doorway_scene.h"
#include "raylib.h"
#include "utility/entity/move_entities.h"
#include "utility/entity/scene_entities.h"
#include "utility/vector2.h" 


void DoorwayScene::Build(
	EcsContext& ecsContext, 
	GameState& gameState,
	ResourceStore& resourceStore
)
{
	Construct::SceneBackgroundEntity(
		LoadTexture("assets/environment/backgrounds/doorway.png"), 
		ecsContext.registry, Doorway
	);

	Construct::MoveRegionEntity(
		ecsContext, gameState, resourceStore, Left, Doorway, CommsRoom, 0.4f
	);
}