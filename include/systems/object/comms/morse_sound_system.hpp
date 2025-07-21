#pragma once
#include "core/scene.hpp"
#include "entt/entity/fwd.hpp"
struct EcsContext;


class MorseSoundSystem
{
public:
	static void Update(
		entt::registry& registry,
		Scene currentScene,
		float deltaTime
	);

};