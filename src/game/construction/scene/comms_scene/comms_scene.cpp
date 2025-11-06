#include "core/data/color.hpp"
#include "core/data/vector2.hpp"
#include "core/runtime/render_context.hpp"
#include "core/runtime/resource_store.hpp"
#include "entt/entity/fwd.hpp"
#include "game/construction/scene/comms_scene/comms_scene.hpp"
#include "game/construction/scene/comms_scene/entity/morse_transceiver_entity.hpp"
#include "game/construction/scene/comms_scene/entity/radio_entity.hpp"
#include "game/construction/scene/comms_scene/object/morse_monitor_object.hpp"
#include "game/construction/scene/comms_scene/object/radar_object.hpp"
#include "game/construction/shared/entity/environment/light_source_entity.hpp"
#include "game/construction/shared/entity/scene/move_region_entity.hpp"
#include "game/construction/shared/entity/scene/scene_background_entity.hpp"
#include "game/state/scene.hpp"
#include "raylib.h"


void CommsScene::Build(
	entt::registry& registry,
	Nc::RenderContext& renderContext,
	GameState& gameState,
	Nc::ResourceStore& resourceStore
)
{
	Construct::RadarObject(registry, resourceStore);
	Construct::MorseMonitorObject(registry, resourceStore);
	Construct::MorseTransceiverEntity(registry, resourceStore);
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

	Nc::Vector2f windowSize = Nc::Vector2f(renderContext.windowSize);
	Nc::Vector2f lightPosition = windowSize * Nc::Vector2f(0.5f, 1.3f);
	float lightRadius = 940.0f * renderContext.renderScale;
	Construct::LightSourceEntity(registry, lightPosition, CommsRoom, LIGHT_COLOR, 1.6f, lightRadius);

}
