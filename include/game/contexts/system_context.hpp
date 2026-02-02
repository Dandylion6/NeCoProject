#pragma once
#include "entt/entity/fwd.hpp"
struct GameEvents;
struct GameState;


namespace Nc
{
class ResourceStore;
}


struct SystemContext final
{
    // ------ Members ------
    entt::registry& registry;
    Nc::ResourceStore& store;
    GameState& game;
    GameEvents& gameEvents;
    float deltaTime;


    // ------ Constructors ------

    SystemContext(
        entt::registry& registry,
        Nc::ResourceStore& store,
        GameState& game,
        GameEvents& gameEvents,
        const float deltaTime
    ) noexcept
        : registry(registry), store(store), game{game}, gameEvents{gameEvents}, deltaTime{deltaTime} {}
};
