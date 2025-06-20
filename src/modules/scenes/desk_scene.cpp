#include "core/scene.h"
#include "entt/entity/fwd.hpp"
#include "modules/scenes/desk_scene.h"
#include "raylib.h"
#include "utility/entity/move_entities.h"
#include "utility/entity/scene_entities.h"
#include "utility/vector2.h"


void DeskScene::Build(
	entt::registry& registry, 
	GameState& gameState, 
	ResourceStore& resourceStore
)
{
	Construct::SceneBackgroundEntity(
		LoadTexture("assets/environment/backgrounds/comms_desk.png"), registry, CommsDesk
	);

	Construct::MoveRegionEntity(
		registry, gameState, resourceStore, Up, CommsDesk, CommsRoom, 0.1f
	);
}