#include "game/system/core/interactive/input_action_system.hpp"

#include "raylib.h"
#include "entt/entity/fwd.hpp"
#include "entt/entity/registry.hpp"
#include "game/component/core/transform_component.hpp"
#include "game/component/core/interactive/input_component.hpp"
#include "game/contexts/system_context.hpp"
#include "game/state/game_state.hpp"
#include "game/state/scene.hpp"


void System::Action::Input::Update(const SystemContext& context)
{
    const auto view = context.registry.view<Component::Action::Input>();
    for (auto [entity, input] : view.each())
    {
        if (context.registry.all_of<Component::Transform>(entity))
        {
            const auto& transform = context.registry.get<Component::Transform>(entity);
            if (transform.boundScene != context.game.currentScene && transform.boundScene == NullScene) continue;
        }
        
        if (IsKeyPressed(input.key)) input.onPressed();
    }
};