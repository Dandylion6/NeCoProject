#pragma once
#include "core/data/vector2.hpp"
#include "game/state/scene.hpp"
#include <cstdint>

using SortIndex = int8_t;


namespace Component
{
/**
 * @brief Defines the world-space transform for scene entities.
 *
 * This component represents the spatial data of an entity that exists within a
 * specific in-game scene. It provides positional, rotational, and
 * dimensional information used to render or simulate entities in the world.
 *
 * - **position**: The entity�s position in world-space coordinates.
 * - **size**: The raw pixel or unit dimensions of the entity�s visual or physical representation.
 * - **offset**: Local positional offset applied before rendering (useful for aligning sprites).
 * - **rotation**: Rotation in radians around the entity�s origin.
 * - **boundScene**: The scene this transform belongs to; only rendered if active.
 * - **index**: The entity�s draw order or layer index.
 *
 * Usage example:
 * 
 * ```cpp
 * constexpr Nc::Vector2f POSITION = Nc::Vector2f(128.0f, 256.0f);
 * constexpr Nc::Vector2f SIZE = Nc::Vector2f(64.0f, 64.0f);
 * constexpr Nc::Vector2f OFFSET = Nc::Vector2f(0.0f, 16.0f);
 *
 * registry.emplace<Component::Transform>(
 *     entity,
 *     Scene::CommsRoom,
 *     POSITION,
 *     SIZE,
 *     OFFSET,
 *     0,
 *     0.0f
 * );
 *
 * registry.emplace<Component::Sprite>(entity, texture);
 * ```
 */
struct Transform
{
	// ------ Members ------

	Nc::Vector2f position = Nc::Vector2f::Zero();
	Nc::Vector2f size = Nc::Vector2f::Zero();
	Nc::Vector2f offset = Nc::Vector2f::Zero();
	float rotation = 0.0f;
	Scene boundScene = CommsRoom;
	SortIndex index = 0;


	// ------ Constructors ------

	constexpr Transform() noexcept = default;
	constexpr Transform(
		Scene boundScene,
		Nc::Vector2f position = Nc::Vector2f::Zero(),
		Nc::Vector2f size = Nc::Vector2f::Zero(),
		Nc:: Vector2f offset = Nc::Vector2f::Zero(),
		SortIndex index = 0,
		float rotation = 0.0f
	) noexcept : 
		boundScene(boundScene),
		position(position),
		size(size),
		offset(offset),
		index(index),
		rotation(rotation)
	{ };
};

}


namespace Component::UI
{
/**
 * @brief Defines the transformation data for UI entities.
 *
 * This component contains all layout and rendering-related positional data
 * used by the UI rendering system. It specifies *where* and *how* an entity
 * is drawn on the screen � including anchoring behavior, pixel dimensions,
 * screen offset, rotation, and rendering order.
 *
 * - **anchor**: normalized (0�1) position relative to the screen or parent container.
 * - **origin**: normalized (0�1) pivot point within the UI element itself.
 * - **size**: the raw pixel dimensions of the element (unscaled).
 * - **offset**: an absolute offset in pixels, applied after anchoring.
 * - **rotation**: clockwise rotation in radians.
 * - **index**: sort order for layered rendering.
 * - **isVisible**: determines whether the element is drawn at all.
 *
 * Usage example:
 * 
 * ```cpp
 * constexpr Nc::Vector2f ANCHOR = Nc::Vector2f(0.5f, 0.5f);  // Center of the screen
 * constexpr Nc::Vector2f ORIGIN = Nc::Vector2f(0.0f, 0.5f);  // Left-center pivot
 * constexpr Nc::Vector2f SIZE = Nc::Vector2f(200.0f, 100.0f); // 200x100px box
 * constexpr Nc::Vector2f OFFSET = Nc::Vector2f(25.0f, 0.0f); // Offset of 25px
 *
 * registry.emplace<Component::UI::Transform>(entity, ANCHOR, ORIGIN, SIZE, OFFSET, 1);
 * registry.emplace<Component::Sprite>(entity, texture);
 * ```
 */
struct Transform
{
	// ------ Members ------

	Nc::Vector2f anchor = Nc::Vector2f::Zero();
	Nc::Vector2f origin = Nc::Vector2f::Zero();
	Nc::Vector2f size = Nc::Vector2f::Zero();
	Nc::Vector2f offset = Nc::Vector2f::Zero();
	float rotation = 0.0f;
	SortIndex index = 0;
	bool isVisible = true;


	// ------ Constructors ------

	constexpr Transform() noexcept = default;
	constexpr Transform(Nc::Vector2f anchor, Nc::Vector2f origin, SortIndex index) noexcept 
		: anchor(anchor), origin(origin), index(index)
	{ }; 


	constexpr Transform(
		Nc::Vector2f anchor,
		Nc::Vector2f origin = Nc::Vector2f::Zero(),
		Nc::Vector2f size = Nc::Vector2f::Zero(),
		Nc::Vector2f offset = Nc::Vector2f::Zero(),
		SortIndex index = 0,
		float rotation = 0.0f
	) noexcept :
		anchor(anchor),
		origin(origin),
		size(size),
		offset(offset),
		index(index),
		rotation(rotation)
	{ };
};

}