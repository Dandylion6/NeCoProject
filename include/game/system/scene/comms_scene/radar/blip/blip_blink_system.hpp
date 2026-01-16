#pragma once
#include "entt/entity/fwd.hpp"

namespace Component
{
struct Transform;
}

namespace Nc
{
struct Tween;
}


namespace System::Blip
{
class Blink final
{
public:
	// ------ Functions ------
	static void Update(entt::registry& registry);

private:
	// ------ Functions ------
	static void UpdateBlips(entt::registry& registry, const Component::Transform& pathTransform);
	static bool BlipShouldAppear(
		entt::registry& registry,
		const Component::Transform& blip,
		const Component::Transform& path,
		const Nc::Tween& tween
	);
};
}

