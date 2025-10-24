#pragma once
#include "components/anomaly/anomaly_roamer_component.hpp"
#include "entt/entity/fwd.hpp"
#include "utility/vector2.hpp"
struct AnomalyState;


class RoamerBehaviourSystem
{
public:
	static void Update(entt::registry& registry, AnomalyState& anomalyState, float deltaTime);
	static Nc::Vector2f GetTargetPosition(Component::AnomalyRoamer::Target target);

};