#pragma once
#include "entt/entity/fwd.hpp"
#include "game/contexts/system_context.hpp"


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
	static void Update(const SystemContext& context) noexcept;
};

}
