#include "game/system/shared/input/move_region_system.hpp"

#include "core/runtime/resource_store.hpp"
#include "entt/entity/registry.hpp"
#include "game/component/core/interactive/click_action_component.hpp"
#include "game/component/shared/input/move_region_component.hpp"
#include "game/construction/ui/shared/entity/move_transition_entity.hpp"
#include "game/contexts/system_context.hpp"
#include "game/state/game_state.hpp"
#include "game/state/scene.hpp"
#include "game/system/ui/move_transition_system.hpp"


void System::Input::MoveRegion::Update(const SystemContext& context)
{
	const auto sceneContext = SceneContext(context.registry, context.store, context.game);

    const auto view = context.registry.view<Component::MoveRegion, Component::Action::Click>();
    for (auto [entity, region, click] : view.each())
    {
        constexpr char TRANSITION_SOUND_PATH[] = "assets/audio/effects/scene_transition.wav";

    	if (!click.justClicked) continue;
		if (context.game.movingToScene != NullScene) continue;

		context.game.movingToScene = region.nextScene;

        UI::MoveTransition::StartMoveScene(sceneContext, region.moveTime);

		const Sound& transitionSound = context.store.CreateSoundHandle(TRANSITION_SOUND_PATH);
		PlaySound(transitionSound);
    }
}
