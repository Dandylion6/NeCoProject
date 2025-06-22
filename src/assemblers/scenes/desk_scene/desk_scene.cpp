#include "assemblers/entities/move_region_entity.h"
#include "assemblers/entities/scene_background_entity.h"
#include "assemblers/scenes/desk_scene/desk_scene.h"
#include "core/scene.h"
#include "entt/entity/fwd.hpp"
#include "raylib.h"
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