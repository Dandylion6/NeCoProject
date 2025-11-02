#include "game/state/anomaly_state.hpp"
#include "game/state/game_state.hpp"
#include "game/system/shared/anomaly/anomaly_attraction_system.hpp"
#include "core/math/interpolation.hpp"
#include "core/data/vector2.hpp"
#include <cmath>


void AnomalyAttractionSystem::Update(GameState& gameState, float deltaTime)
{
	if (!GameState::IsNight(gameState.hour))
	{
		gameState.anomalyState.attractionPercentage = 0.0f;
		return;
	}

	// @brief Attraction decrease per second minimum and maximum values.
	constexpr Nc::Vector2f ATTRACTION_DECAY_RANGE = Nc::Vector2f(0.01f, 0.04f);
	// @brief The percentage range over which the decay interpolates.
	constexpr Nc::Vector2f DECAY_CHANGE_RANGE = Nc::Vector2f(20.0f, 80.0f);

	float decay = Nc::Math::ClampedRemap(DECAY_CHANGE_RANGE, ATTRACTION_DECAY_RANGE, gameState.anomalyState.attractionPercentage);
	gameState.anomalyState.attractionPercentage -= decay * deltaTime;

	// Ensure attraction does not go below base level.
	gameState.anomalyState.attractionPercentage = std::fmaxf(gameState.anomalyState.attractionPercentage, AnomalyState::BASE_ATTRACTION);
}