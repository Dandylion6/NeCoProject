#include "core/data/color.hpp"
#include "core/data/vector2.hpp"
#include "core/runtime/render_context.hpp"
#include "core/runtime/resource_store.hpp"
#include "entt/entity/fwd.hpp"
#include "game/construction/scene/comms_desk_scene/comms_desk_scene.hpp"
#include "game/construction/scene/comms_desk_scene/entity/note_entity.hpp"
#include "game/construction/shared/entity/environment/light_source_entity.hpp"
#include "game/construction/shared/entity/scene/move_region_entity.hpp"
#include "game/construction/shared/entity/scene/scene_background_entity.hpp"
#include "game/state/scene.hpp"
#include "raylib.h"
#include <utility>


void DeskScene::Build(
	entt::registry& registry, 
	Nc::RenderContext& renderContext, 
	GameState& gameState, 
	Nc::ResourceStore& resourceStore
)
{
	Construct::NoteEntity(registry, resourceStore);

	Texture2D texture = resourceStore.GetTexture("assets/environment/backgrounds/comms_desk.png");
	Construct::SceneBackgroundEntity(std::move(texture), registry, CommsDesk);
	Construct::MoveRegionEntity(registry, gameState, resourceStore, Up, CommsDesk, CommsRoom, 0.15f);

	constexpr Nc::Hex LIGHT_COLOR = 0xfee8c8ff;

	Nc::Vector2f windowSize = Nc::Vector2f(renderContext.windowSize);
	Nc::Vector2f lightPosition = windowSize * Nc::Vector2f(0.5f, 0.34f);
	float lightRadius = 620.0f * renderContext.renderScale;
	Construct::LightPointEntity(registry, lightPosition, CommsDesk, LIGHT_COLOR, 0.9f, lightRadius);
}