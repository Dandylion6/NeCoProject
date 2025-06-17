#pragma once
#include "components/core/transform_component.h"
#include "entt/entity/fwd.hpp"
struct Tween;


class BlipBlinkSystem
{
public:
	static void Update(entt::registry& registry);

private:
	static bool BlipShouldAppear(
		Component::Transform blip, 
		Component::Transform path,
		Tween& tween
	);

};