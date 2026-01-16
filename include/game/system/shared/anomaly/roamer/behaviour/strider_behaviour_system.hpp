#pragma once
#include "entt/entity/fwd.hpp"


namespace Component
{
struct Transform;


namespace Anomaly
{
struct Roamer;
}
}


class StriderBehaviourSystem final
{
public:
	// ------ Functions ------
	static void Spawn(entt::registry& registry, entt::entity entity, Component::Anomaly::Roamer& roamer);
	static void Update(
		entt::registry& registry,
		entt::entity entity,
		Component::Transform& transform,
		Component::Anomaly::Roamer& roamer,
		float deltaTime
	);

};
