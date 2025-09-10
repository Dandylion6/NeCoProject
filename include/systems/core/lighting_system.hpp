#pragma once
#include "core/game_state.hpp"
#include "entt/entity/fwd.hpp"
#include "raylib.h"
#include "utility/vector2.hpp"
struct GameState;
struct LightingContext;
class ResourceStore;


class LightingSystem
{
public:
    static void Initialize(LightingContext& context, ResourceStore& resourceStore);

    static void Update(
        entt::registry& registry, LightingContext& context, Shader& lightShader, GameState& gameState, Nc::Vector2f cameraPosition, float deltaTime
    );

private:
    enum LightType: int
    {
        Pointlight,
        Spotlight
    };

};