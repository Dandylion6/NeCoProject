#pragma once
#include "components/anomaly/anomaly_roamer_component.hpp"
#include "components/core/transform_component.hpp"
#include "entt/entity/fwd.hpp"


class StriderBehvaiourSystem
{
public:
	static void Update(
		entt::registry& registry, const entt::entity entity, Component::AnomalyRoamer& roamer, float deltaTime
	);

};