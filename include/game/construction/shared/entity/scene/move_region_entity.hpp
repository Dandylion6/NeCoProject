#pragma once
#include "core/data/vector2.hpp"
#include "entt/entity/fwd.hpp"
#include "game/component/core/transform_component.hpp"
#include "game/state/scene.hpp"
struct GameState;
namespace Nc { class ResourceStore; };


namespace Entity
{

class MoveRegion final
{
public:
	static entt::entity Create(
		entt::registry& registry, 
		Nc::ResourceStore& resourceStore,
		GameState& gameState, 
		const Component::Transform&& transform,
		Scene currentScene, 
		Scene nextScene, 
		float moveTime = 0.2f
	) noexcept;


	static entt::entity Create(
		entt::registry& registry, 
		Nc::ResourceStore& resourceStore,
		GameState& gameState, 
		Direction region, 
		Scene currentScene, 
		Scene nextScene, 
		float moveTime = 0.2f
	) noexcept;

};

}