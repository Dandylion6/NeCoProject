#include "components/core/input_component.hpp"
#include "components/core/transform_component.hpp"
#include "core/scene.hpp"
#include "core/state/game_state.hpp"
#include "entt/entity/fwd.hpp"
#include "entt/entity/registry.hpp"
#include "raylib.h"
#include "systems/core/input_action_system.hpp"


void InputActionSystem::Update(entt::registry& registry, GameState& gameState)
{
    auto view = registry.view<Component::Input>();
    
    for (auto [entity, input] : view.each()) 
    {
        if (registry.all_of<Component::Transform>(entity))
        {
            const Component::Transform& transform = registry.get<const Component::Transform>(entity);
            if (transform.boundScene != gameState.currentScene && transform.boundScene == NullScene) continue;
        }
        
        if (IsKeyPressed(input.key)) input.onPressed();
    }
};