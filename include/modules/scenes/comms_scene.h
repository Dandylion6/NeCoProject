#pragma once
#include "entt/entity/fwd.hpp"
#include "utility/vector2.h"
struct EcsContext;
struct GameState;
class ResourceStore;


enum BlipTweens
{
	BlipFadeIn,
	BlipFadeOut
};


namespace Construct
{
	void MorseTransceiverEntity(EcsContext& ecsContext);

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