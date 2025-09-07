#include "assemblers/entities/light_source_entity.hpp"
#include "assemblers/entities/move_region_entity.hpp"
#include "assemblers/entities/scene_background_entity.hpp"
#include "assemblers/scenes/comms_scene/comms_scene.hpp"
#include "assemblers/scenes/comms_scene/morse_monitor_object.hpp"
#include "assemblers/scenes/comms_scene/morse_transceiver_entity.hpp"
#include "assemblers/scenes/comms_scene/radar_object.hpp"
#include "assemblers/scenes/comms_scene/radio_entity.hpp"
#include "core/render_context.hpp"
#include "core/resource_store.hpp"
#include "core/scene.hpp"
#include "entt/entity/fwd.hpp"
#include "raylib.h"
#include "utility/color.hpp"
#include "utility/vector2.hpp"


void CommsScene::Build(
	entt::registry& registry,
	GameState& gameState,
	ResourceStore& resourceStore
)
{
	Construct::RadarObject(registry);
	Construct::MorseMonitorObject(registry, resourceStore);
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

	constexpr Nc::Hex LIGHT_COLOR = 0xfee8c8ff;

	Nc::Vector2f lightPosition = Nc::Vector2f(RenderContext::DISPLAY_SIZE) * Nc::Vector2f(0.6f, 1.3f);
	Construct::LightSourceEntity(registry, lightPosition, CommsRoom, LIGHT_COLOR, 1.6f, 940.0f);

}
