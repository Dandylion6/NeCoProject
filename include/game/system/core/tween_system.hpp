#pragma once
#include "components/core/tween_component.hpp"
#include "entt/entity/fwd.hpp"
struct GameState;
struct Tween;


class TweenSystem
{
public:
	static void Update(
		entt::registry& registry,
		GameState& gameState,
		float deltaTime
	);

private:
	static void UpdateTweenCollection(
		Component::TweenCollection& collection,
		float deltaTime
	);

	static void TweenEnded(Tween& tween);

};