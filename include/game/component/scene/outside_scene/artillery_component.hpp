#pragma once
#include "core/data/vector2.hpp"


namespace Component
{

/**
 * @brief Represents the artillery cannon entity.
 * 
 * Holds variables for aiming the artillery trajectory and for handling
 * fire state. 
 */
struct Artillery final
{
	// ------ Members ------

	Nc::Vector2f targetPosition = Nc::Vector2f::Zero();
	Nc::Vector2f aimPosition = targetPosition;
	float aimStartupSeconds = 0.0f;
	float fireDelaySeconds = 0.0f;
	bool isReadyToFire = true;
	bool receivedFireRequest = false;


	// ------ Constructors ------

	constexpr Artillery() noexcept = default;
};

}