#pragma once
#include "entt/entity/fwd.hpp"


struct RadarRestartSystem
{
public:
	static void Update(entt::registry& registry, float deltaTime);
	static void OnLeverDrag(entt::registry& registry, const entt::entity entity);
	static void OnLeverRelease(entt::registry& registry, const entt::entity entity);

};