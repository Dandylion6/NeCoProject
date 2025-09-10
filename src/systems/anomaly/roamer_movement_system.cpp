#include "components/anomaly/anomaly_roamer_component.hpp"
#include "components/core/transform_component.hpp"
#include "components/objects/health_component.hpp"
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
			constexpr float SPEED_MODIFIER = 0.2f;

			Nc::Vector2f difference = GameState::BUNKER_POSITION - transform.position;
			Nc::Vector2f direction = difference.Normalized();
			transform.position += direction * roamer.speed * deltaTime * SPEED_MODIFIER;
			
			if (difference.GetSqrDistance() <= 16.0f)
			{
				Component::Health& health = registry.get<Component::Health>(entity);
				health.health = 0;
			}
			break;
		}
		default:
			break;
		}
	}
};