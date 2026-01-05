#include "game/construction/scene/comms_scene/comms_scene.hpp"
#include "core/data/color.hpp"
#include "core/data/vector2.hpp"
#include "core/runtime/render_context.hpp"
#include "core/runtime/resource_store.hpp"
#include "entt/entity/fwd.hpp"
#include "game/construction/scene/comms_scene/entity/morse_transceiver_entity.hpp"
#include "game/construction/scene/comms_scene/entity/radio_entity.hpp"
#include "game/construction/scene/comms_scene/object/morse_monitor_object.hpp"
#include "game/construction/scene/comms_scene/object/radar_object.hpp"
#include "game/construction/shared/entity/environment/light_source_entity.hpp"
#include "game/construction/shared/entity/scene/move_region_entity.hpp"
#include "game/construction/shared/entity/scene/scene_background_entity.hpp"
#include "game/state/scene.hpp"
#include "raylib.h"
#include <utility>


void Structure::CommsScene::Build(
	entt::registry& registry, 
	Nc::ResourceStore& resourceStore, 
	Nc::RenderContext& renderContext, 
	GameState& gameState
) noexcept
{
	constexpr Nc::Hex LIGHT_COLOR = 0xfee8c8ff;

	Object::Radar::Create(registry, resourceStore);
	Object::MorseMonitor::Create(registry, resourceStore);
	Entity::MorseTransceiver::Create(registry, resourceStore);
	Entity::Radio::Create(registry);

	Texture2D sceneTexture = LoadTexture("assets/environment/backgrounds/comms_room.png");
	Entity::SceneBackground::Create(std::move(sceneTexture), registry, CommsRoom);

	Entity::MoveRegion::Create(registry, resourceStore, gameState, Down, CommsRoom, CommsDesk, 0.15f);
	Entity::MoveRegion::Create(registry, resourceStore, gameState, Right, CommsRoom, Doorway, 0.35f);

	Nc::Vector2f windowSize = Nc::Vector2f(renderContext.windowSize);
	Nc::Vector2f lightPosition = windowSize * Nc::Vector2f(0.5f, 1.3f);
	float lightRadius = 940.0f * renderContext.renderScale;

	Entity::LightPoint::Create(registry, CommsRoom, lightPosition, LIGHT_COLOR, 1.6f, lightRadius);
}