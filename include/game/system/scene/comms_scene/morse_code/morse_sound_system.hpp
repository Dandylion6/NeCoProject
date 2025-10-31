#pragma once
#include "game/state/scene.hpp"
#include "entt/entity/fwd.hpp"
struct EcsContext;


class MorseSoundEmitterSystem
{
public:
	static void Update(
		entt::registry& registry,
		Scene currentScene,
		float deltaTime
	);

};