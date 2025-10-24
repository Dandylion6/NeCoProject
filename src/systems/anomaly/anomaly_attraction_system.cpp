#include "core/state/anomaly_state.hpp"
#include "systems/anomaly/anomaly_attraction_system.hpp"
#include "utility/interpolation.hpp"
#include "utility/vector2.hpp"
#include <cmath>


void AnomalyAttractionSystem::Update(AnomalyState& anomalyState, float deltaTime)
{
	// @brief Attraction decrease per second minimum and maximum values.
	constexpr Nc::Vector2f ATTRACTION_DECAY_RANGE = Nc::Vector2f(0.01f, 0.04f);
	// @brief The percentage range over which the decay interpolates.
	constexpr Nc::Vector2f DECAY_CHANGE_RANGE = Nc::Vector2f(20.0f, 80.0f);

	float decay = Math::ClampedRemap(DECAY_CHANGE_RANGE, ATTRACTION_DECAY_RANGE, anomalyState.attractionPercentage);
	anomalyState.attractionPercentage -= decay * deltaTime;

	// Ensure attraction does not go below base level.
	anomalyState.attractionPercentage = std::fmaxf(anomalyState.attractionPercentage, AnomalyState::BASE_ATTRACTION);
}