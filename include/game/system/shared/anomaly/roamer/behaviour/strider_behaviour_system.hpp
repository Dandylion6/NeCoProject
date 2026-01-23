#pragma once
#include "entt/entity/fwd.hpp"


namespace Component
{
struct Transform;


namespace Anomaly
{
struct Roamer;
}
}


namespace System::Anomaly::Roamer
{

class Strider final
{
public:
	// ------ Functions ------
	static void Spawn(entt::registry& registry, entt::entity entity) noexcept;
	static void Update(
		entt::registry& registry,
		entt::entity entity,
		Component::Transform& transform,
		const Component::Anomaly::Roamer& roamer,
		float deltaTime
	) noexcept;
};

}
