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
    constexpr Nc::Vector2f LIGHT_POSITION = Nc::Vector::Modulate(
        Nc::Vector2f(0.5f, 0.36f),
        Nc::Vector2f(Nc::RENDER_RESOLUTION)
    );
	constexpr float LIGHT_RADIUS = 800.0f;
	constexpr float MOVE_TIME = 0.16f;

	const SceneContext sceneContext = SceneContext(context.registry, context.store, context.game);

	Entity::Note::Create(sceneContext);

	const Texture2D& texture = context.store.GetTexture(SCENE_TEXTURE_PATH);
	Entity::SceneBackground::Create(context.registry, texture, CommsDesk);

	Entity::MoveRegion::Create(sceneContext, Up, CommsDesk, CommsRoom, MOVE_TIME);
	Entity::LightPoint::Create(context.registry, CommsDesk, LIGHT_POSITION, 300.0f, Nc::RGBa(LIGHT_COLOR), 3.2f, LIGHT_RADIUS);
}
