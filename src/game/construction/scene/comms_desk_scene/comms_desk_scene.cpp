#include "game/construction/scene/comms_desk_scene/comms_desk_scene.hpp"
#include "core/data/color.hpp"
#include "core/data/vector2.hpp"
#include "core/runtime/render_context.hpp"
#include "core/runtime/resource_store.hpp"
#include "entt/entity/fwd.hpp"
#include "game/construction/scene/comms_desk_scene/entity/note_entity.hpp"
#include "game/construction/shared/entity/environment/light_source_entity.hpp"
#include "game/construction/shared/entity/scene/move_region_entity.hpp"
#include "game/construction/shared/entity/scene/scene_background_entity.hpp"
#include "game/state/scene.hpp"
#include "raylib.h"
#include <utility>


void Structure::DeskScene::Build(
	entt::registry& registry, 
	Nc::ResourceStore& resourceStore,
	Nc::RenderContext& renderContext, 
	GameState& gameState
) noexcept
{
	constexpr Nc::Hex LIGHT_COLOR = 0xfee8c8ff;

	Entity::Note::Create(registry, resourceStore);

	Texture2D texture = resourceStore.GetTexture("assets/environment/backgrounds/comms_desk.png");
	Entity::SceneBackground::Create(std::move(texture), registry, CommsDesk);
	Entity::MoveRegion::Create(registry, resourceStore, gameState, Up, CommsDesk, CommsRoom, 0.15f);

	Nc::Vector2f windowSize = Nc::Vector2f(renderContext.windowSize);
	Nc::Vector2f lightPosition = windowSize * Nc::Vector2f(0.5f, 0.34f);
	float lightRadius = 620.0f * renderContext.renderScale;

	Entity::LightPoint::Create(registry, CommsDesk, lightPosition, LIGHT_COLOR, 0.9f, lightRadius);
}