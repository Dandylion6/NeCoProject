#pragma once


namespace Component::Anomaly
{
/**
 * @brief Represents a Strider anomaly roamer entity.
 * 
 * Holds extra variables for simple movement behaviour, like 
 * `moveSpeed` and `currentMoveSpeed`. This component might be
 * used for multiple types of roamers.
 * 
 * Usage example:
 * 
 * ```cpp
 * constexpr float MOVE_SPEED = 2.0f;
 * ...
 * registry.emplace<Component::Blip>(entity, ...);
 * registry.emplace<Component::Anomaly::Roamer>(entity, ...);
 * registry.emplace<Component::Anomaly::Strider>(entity, MOVE_SPEED);
 * ```
 */
struct Strider
{
	// ------ Members ------

	float moveSpeed = 0.1f;
	// TODO: Use this variable instead of movespeed, to allow for speed modifications (e.g. slowing effects)
	float currentMoveSpeed = moveSpeed;


	// ------ Constructors ------

	constexpr Strider() noexcept = default;
	constexpr Strider(float moveSpeed) noexcept : 
		moveSpeed(moveSpeed), currentMoveSpeed(moveSpeed)
	{ };
};

}