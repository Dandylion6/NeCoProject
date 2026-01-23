#pragma once
#include "entt/entity/fwd.hpp"
#include "core/data/vector2.hpp"
#include "game/component/shared/anomaly/roamer/anomaly_roamer_component.hpp"
#include "game/component/core/transform_component.hpp"
#include "game/component/shared/stat/health_component.hpp"
struct SystemContext;
struct GameState;


namespace System::Anomaly::Roamer
{


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
	);

	static void UpdateArtilleryRoamer(
		entt::registry& registry,
		const Component::Transform& transform,
		Component::Health& health
	);

	// @brief Checks if the roamer can kill the target.
	static bool CanKill(Nc::Vector2f roamerPosition, Nc::Vector2f targetPosition, float killDistance = 6.0f);
	static void KillArtillery(entt::registry& registry);

};

}
