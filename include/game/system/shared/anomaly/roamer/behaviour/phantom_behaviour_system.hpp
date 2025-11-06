#pragma once
#include "game/component/shared/anomaly/roamer/anomaly_roamer_component.hpp"
#include "game/component/core/transform_component.hpp"
#include "entt/entity/fwd.hpp"
struct AnomalyState;
namespace Nc { class ResourceStore; };


class PhantomBehaviourSystem
{
public:
	static void Spawn(
		entt::registry& registry, AnomalyState& anomalyState, const entt::entity entity, Component::Anomaly::Roamer& roamer
	);
	static void Update(
		entt::registry& registry,
		const entt::entity entity,
		Component::Transform& transform,
		Component::Anomaly::Roamer& roamer,
		float deltaTime
	);

};