#pragma once
#include "core/data/vector2.hpp"
#include "entt/entity/fwd.hpp"


namespace Component
{
struct Transform;
struct Health;
}


struct SystemContext;
struct GameState;


namespace System::Anomaly::Roamer
{

// TODO: Refactor this.
class Kill final
{
public:
	// ------ Functions ------
	static void Update(const SystemContext& context) noexcept;

private:
	// ------ Functions ------
	static void UpdateBunkerRoamer(
		const SystemContext& context,
		const Component::Transform& transform,
		Component::Health& health
	) noexcept;

	static void UpdateArtilleryRoamer(
		entt::registry& registry,
		const Component::Transform& transform,
		Component::Health& health
	) noexcept;

	static bool CanKill(Nc::Vector2f roamerPosition, Nc::Vector2f targetPosition, float killDistance = 6.0f) noexcept;
	static void KillArtillery(entt::registry& registry) noexcept;
};

}
