#include "components/anomaly/anomaly_roamer_component.hpp"
#include "components/core/transform_component.hpp"
#include "core/game_state.hpp"
#include "entt/entity/fwd.hpp"
#include "entt/entity/registry.hpp"
#include "systems/anomaly/roamer_movement_system.hpp"
#include "utility/vector2.hpp"


void RoamerMovementSystem::Update(
	entt::registry& registry, float deltaTime
)
{
	auto view = registry.view<const Component::AnomalyRoamer, Component::Transform>();
	for (auto [entity, roamer, transform] : view.each())
	{
		switch (roamer.behaviour)
		{
		case Component::AnomalyRoamer::Strider:
		{
			Nc::Vector2f direction = (GameState::BUNKER_POSITION - transform.position).Normalized();
			transform.position += direction * roamer.speed * deltaTime;
			break;
		}
		default:
			break;
		}
	}
};