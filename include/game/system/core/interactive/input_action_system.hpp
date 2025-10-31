#pragma once
#include "entt/entity/fwd.hpp"
struct GameState;


class InputActionSystem 
{
public:
    static void Update(
        entt::registry& registry,
        GameState& gameState
    );

};