#pragma once
#include "entt/entity/fwd.hpp"
#include "utility/vector2.h"
struct EcsContext;
struct GameState;
class ResourceStore;


namespace Construct
{
	void MorseTransceiverEntity(entt::registry& registry);

	entt::entity RadarPathEntity(entt::registry& registry);

	void RadarObject(entt::registry& registry);

	void RadarBlipEntity(entt::registry& registry, Nc::Vector2f position);
}

namespace CommsScene
{
	void Build(
		EcsContext& ecsContext, 
		GameState& gameState, 
		ResourceStore& resourceStore
	);
}