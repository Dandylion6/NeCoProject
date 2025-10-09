#pragma once
#include "components/core/transform_component.hpp"
#include "components/objects/comms/radar.hpp"
#include "entt/entity/fwd.hpp"
struct Tween;


class BlipBlinkSystem
{
public:
	static void Update(entt::registry& registry);

private:
	static void UpdateBlips(entt::registry& registry, const Component::Transform& pathTransform);
	static bool BlipShouldAppear(Component::Transform blip, Component::Transform path, Tween& tween);

};