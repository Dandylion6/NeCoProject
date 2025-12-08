#pragma once
#include "core/data/vector2.hpp"
#include "entt/entity/fwd.hpp"
#include "game/component/shared/anomaly/roamer/anomaly_roamer_component.hpp"
struct AnomalyState;


class RoamerBehaviourSystem
{
public:
	static void Update(entt::registry& registry, AnomalyState& anomalyState, float deltaTime);
	static Nc::Vector2f GetTargetPosition(RoamerTarget target);

};