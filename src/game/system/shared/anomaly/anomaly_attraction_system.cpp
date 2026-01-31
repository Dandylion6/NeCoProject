#include "game/system/shared/anomaly/anomaly_attraction_system.hpp"

#include <cmath>

#include "core/data/vector2.hpp"
#include "core/math/interpolation.hpp"
#include "game/contexts/system_context.hpp"
#include "game/state/anomaly_state.hpp"
#include "game/state/game_state.hpp"


void System::Anomaly::Attraction::Update(const SystemContext& context, AnomalyState& anomaly)
{
	// Attraction decrease per second minimum and maximum values.
	constexpr Nc::Vector2f ATTRACTION_DECAY_RANGE = Nc::Vector2f(0.1f, 0.4f);
	// The percentage range over which the decay interpolates.
	constexpr Nc::Vector2f DECAY_CHANGE_RANGE = Nc::Vector2f(20.0f, 80.0f);

	if (!GameState::IsNight(context.game.hour))
	{
		// Daytime is completely safe.
		anomaly.attractionPercentage = 0.0f;
		return;
	}

	// NOTE: Attraction currently reduces faster with a higher percentage, for now this is to make the game more fair.
	const float decay = Nc::Math::ClampedRemap(DECAY_CHANGE_RANGE, ATTRACTION_DECAY_RANGE, anomaly.attractionPercentage);
	anomaly.attractionPercentage -= decay * context.deltaTime;

	// Ensure attraction does not go below base level.
	anomaly.attractionPercentage = std::fmaxf(anomaly.attractionPercentage, AnomalyState::BASE_ATTRACTION);
}
