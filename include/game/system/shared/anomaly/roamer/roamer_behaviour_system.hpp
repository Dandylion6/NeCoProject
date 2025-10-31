#pragma once
#include "game/component/shared/anomaly/roamer/anomaly_roamer_component.hpp"
#include "entt/entity/fwd.hpp"
#include "core/data/vector2.hpp"
struct AnomalyState;


class RoamerBehaviourSystem
{
public:
	static void Update(entt::registry& registry, AnomalyState& anomalyState, float deltaTime);
	static Nc::Vector2f GetTargetPosition(Component::Anomaly::Roamer::Target target);

};