#pragma once
#include <array>
#include <cstdint>
#include <utility>

#include "core/data/tween.hpp"


namespace Component
{
/**
 * @brief Holds up to 4 tweens for an entity.
 * 
 * Using a collection is the best due to entities usually requiring more
 * than one tween animation.
 * 
 * Usage example:
 * 
 * @code
 * enum Tweens
 * {
 *	Blinking,
 *	OtherAnimation,
 *	...
 * }
 * 
 * Component::Sprite& sprite = registry.emplace<Component::Sprite>(entity, ...);
 * Component::TweenCollection& collection = registry.emplace<Component::TweenCollection>(entity);
 * 
 * Nc::Tween& blinkTween = collection.at(Blinking);
 * blinkTween.value = &sprite.alpha;
 * blinkTween.start = 1.0f;
 * blinkTween.end = 0.0f;
 * blinkTween.duration = 3.0f;
 * blinkTween.easing = Linear;
 * blinkTween.onComplete = [&tween]()
 * {
 *	Nc::Tween::Replay(tween);
 * }
 * @endcode
 */
struct TweenCollection final
{
	// ------ Members ------

	static constexpr uint8_t MAX_TWEENS = 4u;
	std::array<Nc::Tween, MAX_TWEENS> tweens{ };


	// ------ Constructors ------

	constexpr TweenCollection() = default;
	explicit TweenCollection(std::array<Nc::Tween, MAX_TWEENS>&& tweens) noexcept : tweens(std::move(tweens)) { }
};

}
