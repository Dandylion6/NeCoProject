#pragma once
#include "core/data/vector2.hpp"
#include "entt/entity/fwd.hpp"
#include "game/state/scene.hpp"


struct SceneContext;
namespace Component
{
struct Transform;
}


namespace Entity
{

class MoveRegion final
{
public:
	// ------ Functions ------
	static entt::entity Create(
		const SceneContext& context,
		const Component::Transform& transform,
		Scene nextScene,
		float moveTime = 0.2f
	) noexcept;

	static entt::entity Create(
		const SceneContext& context,
		Direction region,
		Scene currentScene,
		Scene nextScene,
		float moveTime = 0.2f
	) noexcept;

};

}
