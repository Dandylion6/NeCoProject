#pragma once
#include "components/core/tween_component.h"
#include "entt/entity/fwd.hpp"
struct Tween;


class TweenSystem
{
public:
	static void Update(
		entt::registry& registry, 
		float deltaTime
	);

private:
	static void UpdateTweenCollection(
		Component::TweenCollection& collection,
		float deltaTime
	);

	static void TweenEnded(Tween& tween);

};