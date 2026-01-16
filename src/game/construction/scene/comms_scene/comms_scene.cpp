#include "game/construction/scene/comms_scene/comms_scene.hpp"

#include "raylib.h"
#include "core/data/color.hpp"
#include "core/data/vector2.hpp"
#include "core/math/vector_math.hpp"
#include "core/runtime/render_context.hpp"
#include "game/construction/scene/comms_scene/entity/morse_transceiver_entity.hpp"
#include "game/construction/scene/comms_scene/entity/radio_entity.hpp"
#include "game/construction/scene/comms_scene/object/morse_monitor_object.hpp"
#include "game/construction/scene/comms_scene/object/radar_object.hpp"
#include "game/construction/shared/entity/environment/light_source_entity.hpp"
#include "game/construction/shared/entity/scene/move_region_entity.hpp"
#include "game/construction/shared/entity/scene/scene_background_entity.hpp"
#include "game/state/scene.hpp"


void Structure::CommsScene::Build(const BuildContext& context) noexcept
{
	constexpr char SCENE_TEXTURE_PATH[] = "assets/environment/backgrounds/comms_room.png";
	constexpr auto LIGHT_COLOR = Nc::Hex(0xfee8c8ff);

	const auto sceneContext = SceneContext(context.registry, context.store, context.game);

	Object::Radar::Create(sceneContext);
	Object::MorseMonitor::Create(sceneContext);
	Entity::MorseTransceiver::Create(sceneContext);
	Entity::Radio::Create(sceneContext);

	const Texture2D& sceneTexture = LoadTexture(SCENE_TEXTURE_PATH);
	Entity::SceneBackground::Create(context.registry, sceneTexture, CommsRoom);

	Entity::MoveRegion::Create(sceneContext, Down, CommsRoom, CommsDesk, 0.15f);
	Entity::MoveRegion::Create(sceneContext, Right, CommsRoom, Doorway, 0.35f);

	const Nc::Vector2f windowSize = Nc::Vector2f(context.renderContext.windowSize);
	const Nc::Vector2f lightPosition = Nc::Vector::Modulate(windowSize, Nc::Vector2f(0.5f, 1.3f));
	const float lightRadius = 940.0f * context.renderContext.renderScale;

	Entity::LightPoint::Create(context.registry, CommsRoom, lightPosition, Nc::RGBa(LIGHT_COLOR), 1.6f, lightRadius);
}
