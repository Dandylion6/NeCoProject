#include "core/ecs_context.h"
#include "core/scene.h"
#include "modules/scenes/desk_scene.h"
#include "raylib.h"
#include "utility/entity/move_entities.h"
#include "utility/entity/scene_entities.h"
#include "utility/vector2.h"


void DeskScene::Build(
	EcsContext& ecsContext, 
	GameState& gameState, 
	ResourceStore& resourceStore
)
{
	Construct::SceneBackgroundEntity(
		LoadTexture("assets/environment/backgrounds/comms_desk.png"), 
		ecsContext.registry, CommsDesk
	);

	Construct::MoveRegionEntity(
		ecsContext, gameState, resourceStore, Up, CommsDesk, CommsRoom, 0.1f
	);
}