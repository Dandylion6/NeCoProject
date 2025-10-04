#pragma once
#include "components/anomaly/anomaly_roamer_component.hpp"
#include "entt/entity/fwd.hpp"
#include "utility/vector2.hpp"


class RoamerBehaviourSystem
{
public:
	static void Update(entt::registry& registry, float deltaTime);

	static Nc::Vector2f GetTargetPosition(Component::AnomalyRoamer::Target target);

};