#include "game/construction/scene/doorway_scene/doorway_scene.hpp"

#include "raylib.h"
#include "core/data/color.hpp"
#include "core/data/vector2.hpp"
#include "core/math/vector_math.hpp"
#include "core/runtime/render_context.hpp"
#include "core/runtime/resource_store.hpp"
#include "entt/entity/fwd.hpp"
#include "game/construction/shared/entity/environment/light_source_entity.hpp"
#include "game/construction/shared/entity/scene/move_region_entity.hpp"
#include "game/construction/shared/entity/scene/scene_background_entity.hpp"
#include "game/contexts/build_context.hpp"
#include "game/contexts/scene_context.hpp"
#include "game/state/scene.hpp"


void Structure::DoorwayScene::Build(const BuildContext& context) noexcept
{
	constexpr char FILE_PATH[] = "assets/environment/backgrounds/doorway.png";
	constexpr auto LIGHT_COLOR = Nc::Hex(0xfee8c8ff);

	const Texture2D& backgroundText = context.store.GetTexture(FILE_PATH);
	Entity::SceneBackground::Create(context.registry, backgroundText, Doorway);

	const SceneContext sceneContext = SceneContext(context.registry, context.store, context.game);
	Entity::MoveRegion::Create(sceneContext, Left, Doorway, CommsRoom, 0.4f);

	const auto windowSize = Nc::Vector2f(context.renderContext.windowSize);
	const Nc::Vector2f lightPosition = Nc::Vector::Modulate(windowSize, Nc::Vector2f(0.5f, 1.3f));
	const float lightRadius = 940.0f * context.renderContext.renderScale;

	Entity::LightPoint::Create(context.registry, Doorway, lightPosition, Nc::RGBa(LIGHT_COLOR), 1.6f, lightRadius);
}
