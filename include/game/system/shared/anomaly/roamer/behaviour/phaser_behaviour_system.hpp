#pragma once
#include "game/component/shared/anomaly/roamer/anomaly_roamer_component.hpp"
#include "game/component/core/transform_component.hpp"
#include "entt/entity/fwd.hpp"


namespace System::Anomaly::Roamer
{

class Phaser final
{
public:
	// ------ Functions ------
	static void Spawn(entt::registry& registry, entt::entity entity) noexcept;
	static void Update(
		entt::registry& registry,
		entt::entity entity,
		Component::Transform& transform,
		Component::Anomaly::Roamer& roamer,
		float deltaTime
	) noexcept;

};

}

