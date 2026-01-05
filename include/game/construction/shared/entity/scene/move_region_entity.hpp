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
	static const entt::entity Create(
		entt::registry& registry, 
		GameState& gameState, 
		Nc::ResourceStore& resourceStore,
		const Component::Transform&& transform,
		Scene currentScene, 
		Scene nextScene, 
		float moveTime = 0.2f
	) noexcept;

	static const entt::entity Create(
		entt::registry& registry, 
		GameState& gameState, 
		Nc::ResourceStore& resourceStore,
		Direction region, 
		Scene currentScene, 
		Scene nextScene, 
		float moveTime = 0.2f
	) noexcept;

};

}