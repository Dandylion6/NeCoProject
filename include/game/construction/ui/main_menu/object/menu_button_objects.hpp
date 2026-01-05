#pragma once
#include "entt/entity/fwd.hpp"
class Game;
struct GameState;
namespace Nc { class ResourceStore; };


namespace Object
{

class PlayButton final
{
public:
	static void Create(
		Game& game, 
		entt::registry& registry, 
		GameState& gameState, 
		Nc::ResourceStore& resourceStore
	) noexcept;

};


class SettingsButton final
{
public:
	static void Create(
		entt::registry& registry, 
		GameState& gameState, 
		Nc::ResourceStore& resourceStore
	) noexcept;

};


class ExitButton final
{
public:
	static void Create(
		entt::registry& registry, 
		GameState& gameState, 
		Nc::ResourceStore& resourceStore
	) noexcept;

};

}