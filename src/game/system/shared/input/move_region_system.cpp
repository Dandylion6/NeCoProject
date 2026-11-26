#include "game/system/shared/input/move_region_system.hpp"

#include "core/runtime/resource_store.hpp"
#include "entt/entity/registry.hpp"
#include "game/component/core/audio/audio_component.hpp"
#include "game/component/core/audio/audio_emitter_component.hpp"
#include "game/component/core/interactive/click_action_component.hpp"
#include "game/component/shared/input/move_region_component.hpp"
#include "game/construction/ui/shared/entity/move_transition_entity.hpp"
#include "game/contexts/system_context.hpp"
#include "game/state/game_state.hpp"
#include "game/state/scene.hpp"
#include "game/system/core/audio/audio_emitter_system.hpp"
#include "game/system/ui/move_transition_system.hpp"


void System::Input::MoveRegion::Update(const SystemContext& context)
{
	const auto sceneContext = SceneContext(context.registry, context.store, context.game);
    const auto view = context.registry.view<Component::MoveRegion, Component::Action::Click, Component::AudioEmitter>();
    for (const auto& [entity, region, click, emitter] : view.each())
    {
    	if (!click.justClicked) continue;
		if (context.game.movingToScene != NullScene) continue;

		context.game.movingToScene = region.nextScene;

        UI::MoveTransition::StartMoveScene(sceneContext, region.moveTime);
		Audio::Emitter::PlayEmitter(emitter);
    }
}
