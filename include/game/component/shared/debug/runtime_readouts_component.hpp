#pragma once
#include "game/utility/morse_code.hpp"
#include <cstdint>
#include <deque>
#include <string>


namespace Component::Debug
{
/**
 * @brief Contains transient values intended for inspection
 * and visualization (e.g. debug UI).
 *
 * Stored in the ECS registry context.
 * Written by gameplay systems, read by debug UI.
 * 
 * Usage example:
 * ```cpp
 * constexpr float TIME_SCALE = 1.3f;
 * 
 * registry.emplace<Component::Debug::RuntimeReadouts>(entity);
 * 
 * auto view = registry.view<Component::Debug::RuntimeReadouts>();
 * for (auto [entity, readouts] : view.each())
 *  	readouts.timeScale = TIME_SCALE;
 * ```
 */
struct RuntimeReadouts final
{
	std::deque<int16_t> fpsHistory = std::deque<int16_t>(32u, 0);
	std::string receiverMessage = "";
	MorseCode::Pulse pulse = MorseCode::Invalid;
	float radarStabilityPercentage = 0.0f;
	float timeScale = 1.0f;
};

}