#include "components/core/transform_component.hpp"
#include "entt/entity/fwd.hpp"
#include "entt/entity/registry.hpp"
#include "raylib.h"
#include "systems/object/breaker/radar_restart_system.hpp"
#include "utility/vector2.hpp"


void RadarRestartSystem::Update(entt::registry& registry, float deltaTime)
{

}


void RadarRestartSystem::OnLeverDrag(entt::registry& registry, const entt::entity entity)
{
	Nc::Vector2f mouseDelta = GetMouseDelta();
	Component::Transform& transform = registry.get<Component::Transform>(entity);
	transform.position.y += mouseDelta.y;
}


void RadarRestartSystem::OnLeverRelease(entt::registry& registry, const entt::entity entity)
{

}
