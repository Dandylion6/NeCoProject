#pragma once
#include "components/anomaly/anomaly_roamer_component.hpp"
#include "components/core/transform_component.hpp"
#include "entt/entity/fwd.hpp"


class StriderBehaviourSystem
{
public:
	static void Spawn(
		entt::registry& registry, const entt::entity entity, Component::AnomalyRoamer& roamer
	);
	static void Update(
		entt::registry& registry, 
		const entt::entity entity, 
		Component::Transform& transform, 
		Component::AnomalyRoamer& roamer, 
		float deltaTime
	);

};