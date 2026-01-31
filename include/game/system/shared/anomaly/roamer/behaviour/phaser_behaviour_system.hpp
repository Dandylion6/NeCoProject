#pragma once
#include "entt/entity/fwd.hpp"


struct SystemContext;


namespace System::Anomaly::Roamer
{

class Phaser final
{
public:
	// ------ Functions ------
	static void Spawn(entt::registry& registry, entt::entity entity) noexcept;
	static void Update(const SystemContext& context) noexcept;

private:
	static void ChangeToStrider(entt::registry& registry, entt::entity entity) noexcept;

};

}

