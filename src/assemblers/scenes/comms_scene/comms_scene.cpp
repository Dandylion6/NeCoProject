#include "assemblers/entities/move_region_entity.h"
#include "assemblers/entities/scene_background_entity.h"
#include "assemblers/scenes/comms_scene/comms_scene.h"
#include "assemblers/scenes/comms_scene/morse_transceiver_entity.h"
#include "assemblers/scenes/comms_scene/radar_object.h"
#include "assemblers/scenes/comms_scene/radio_entity.h"
#include "core/resource_store.h"
#include "core/scene.h"
#include "entt/entity/fwd.hpp"
#include "raylib.h"
#include "utility/vector2.h"


void CommsScene::Build(
	entt::registry& registry,
	GameState& gameState,
	ResourceStore& resourceStore
)
{
	Construct::RadarObject(registry);
	Construct::MorseTransceiverEntity(registry);
	Construct::RadioEntity(registry);

	Construct::SceneBackgroundEntity(
		LoadTexture("assets/environment/backgrounds/comms_room.png"), registry, CommsRoom
	);
	Construct::MoveRegionEntity(
		registry, gameState, resourceStore, Down, CommsRoom, CommsDesk, 0.15f
	);
	Construct::MoveRegionEntity(
		registry, gameState, resourceStore, Right, CommsRoom, Doorway, 0.35f
	);

}
