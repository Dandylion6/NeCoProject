#pragma once
#include "core/data/vector2.hpp"
#include "entt/entity/fwd.hpp"
#include "raylib.h"
struct GameState;
namespace Nc 
{ 
    struct LightingContext;
    class ResourceStore; 
};


class LightingSystem
{
public:
    static void Initialize(
        Nc::LightingContext& context, 
        Nc::ResourceStore& resourceStore
    );

    static void Update(
        entt::registry& registry, 
        Nc::LightingContext& context,
        const Shader& lightShader, 
        GameState& gameState, 
        Nc::Vector2f cameraPosition, 
        float deltaTime
    );

private:
    enum LightType: int
    {
        Pointlight,
        Spotlight
    };

};