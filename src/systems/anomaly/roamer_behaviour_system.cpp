#include "components/anomaly/anomaly_roamer_component.hpp"
#include "components/core/transform_component.hpp"
#include "core/game_state.hpp"
#include "entt/entity/fwd.hpp"
#include "entt/entity/registry.hpp"
#include "systems/anomaly/roamer/strider_behaviour_system.hpp"
#include "systems/anomaly/roamer_behaviour_system.hpp"
#include "utility/vector2.hpp"


void RoamerBehaviourSystem::Update(
	entt::registry& registry, float deltaTime
)
{
	auto view = registry.view<Component::AnomalyRoamer>();
	for (auto [entity, roamer] : view.each())
	{
		switch (roamer.behaviour)
		{
		case Component::AnomalyRoamer::Strider:
		{
			StriderBehvaiourSystem::Update(registry, entity, roamer, deltaTime);
			break;
		}
		default:
			break;
		}
	}
};


Nc::Vector2f RoamerBehaviourSystem::GetTargetPosition(Component::AnomalyRoamer::Target target)
{
	switch (target)
	{
	case Component::AnomalyRoamer::Target::Bunker:
		return GameState::BUNKER_POSITION;
	case Component::AnomalyRoamer::Target::Artillery:
		return GameState::ARTILLERY_POSITION;
	}
}