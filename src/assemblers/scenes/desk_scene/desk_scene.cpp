#include "assemblers/entities/light_source_entity.hpp"
#include "assemblers/entities/move_region_entity.hpp"
#include "assemblers/entities/scene_background_entity.hpp"
#include "assemblers/scenes/desk_scene/desk_scene.hpp"
#include "assemblers/scenes/desk_scene/note_entity.hpp"
#include "core/render_context.hpp"
#include "core/scene.hpp"
#include "entt/entity/fwd.hpp"
#include "raylib.h"
#include "utility/color.hpp"
#include "utility/vector2.hpp"


void DeskScene::Build(
	entt::registry& registry, 
	GameState& gameState, 
	ResourceStore& resourceStore
)
{
	Construct::NoteEntity(registry, resourceStore);

	Construct::SceneBackgroundEntity(
		LoadTexture("assets/environment/backgrounds/comms_desk.png"), registry, CommsDesk
	);

	Construct::MoveRegionEntity(
		registry, gameState, resourceStore, Up, CommsDesk, CommsRoom, 0.15f
	);

	constexpr Nc::Hex LIGHT_COLOR = 0xfee8c8ff;

	Nc::Vector2f displaySize = RenderContext::DISPLAY_SIZE;
	Nc::Vector2f lightPosition = displaySize * Nc::Vector2f(0.5f, 0.34f);
	Construct::LightSourceEntity(registry, lightPosition, CommsDesk, LIGHT_COLOR, 0.9f, 600.0f);
}