#include "game/construction/scene/comms_desk_scene/comms_desk_scene.hpp"

#include "raylib.h"
#include "core/data/color.hpp"
#include "core/data/vector2.hpp"
#include "core/math/vector_math.hpp"
#include "core/runtime/render_context.hpp"
#include "core/runtime/resource_store.hpp"
#include "entt/entity/fwd.hpp"
#include "game/construction/scene/comms_desk_scene/entity/note_entity.hpp"
#include "game/construction/shared/entity/environment/light_source_entity.hpp"
#include "game/construction/shared/entity/scene/move_region_entity.hpp"
#include "game/construction/shared/entity/scene/scene_background_entity.hpp"
#include "game/contexts/build_context.hpp"
#include "game/contexts/scene_context.hpp"
#include "game/state/scene.hpp"


void Structure::DeskScene::Build(const BuildContext& context) noexcept
{
	constexpr char SCENE_TEXTURE_PATH[] = "assets/environment/backgrounds/comms_desk.png";
	constexpr auto LIGHT_COLOR = Nc::Hex(0xfee8c8ff);
	constexpr float MOVE_TIME = 0.16f;

	const SceneContext sceneContext = SceneContext(context.registry, context.store, context.game);

	Entity::Note::Create(sceneContext);

	const Texture2D& texture = context.store.GetTexture(SCENE_TEXTURE_PATH);
	Entity::SceneBackground::Create(context.registry, texture, CommsDesk);

	Entity::MoveRegion::Create(sceneContext, Up, CommsDesk, CommsRoom, MOVE_TIME);

	const Nc::Vector2f windowSize = Nc::Vector2f(context.renderContext.windowSize);
	const Nc::Vector2f lightPosition = Nc::Vector::Modulate(windowSize, Nc::Vector2f(0.5f, 0.34f));
	const float lightRadius = 620.0f * context.renderContext.renderScale;

	Entity::LightPoint::Create(context.registry, CommsDesk, lightPosition, Nc::RGBa(LIGHT_COLOR), 0.9f, lightRadius);
}
