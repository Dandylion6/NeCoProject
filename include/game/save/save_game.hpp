#pragma once
#include "entt/entity/fwd.hpp"
#include "game/save/save_result.hpp"
#include <array>
#include <cstdint>
class Game;
struct GameState;


namespace Save
{

Result GameToDisk(entt::registry& registry, const GameState& gameState);

}


namespace Load
{

Result GameFromDisk(entt::registry& registry, GameState& gameState);

}