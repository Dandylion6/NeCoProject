#pragma once
#include "game/component/scene/comms_scene/morse_components.hpp"
#include "game/state/scene.hpp"
#include "entt/entity/fwd.hpp"
#include "game/utility/morse_code.hpp"
#include <cstdint>
struct AnomalyState;
struct GameState;
struct Settings::Morse;


class MorseTransceiverSystem
{
public:
	static void Update(
		entt::registry& registry, 
		GameState& gameState, 
		Settings::Morse settings,
		float deltaTime
	);

private:
	static void InputChanged(
		Component::Morse::Transceiver& transceiver, Settings::Morse settings
	);

	static void TryEndCharacter(
		entt::registry& registry, AnomalyState& anomalyState, Component::Morse::Transceiver& transceiver, Settings::Morse settings
	);

	static void TransmitCharacter(entt::registry& registry, AnomalyState& anomalyState, char character);

	static void RecordPulse(
		Component::Morse::Transceiver& transceiver, Settings::Morse settings
	);

	static void ClearTransceiver(Component::Morse::Transceiver& transceiver);
	static MorseCode::Pulse GetPulseType(
		float intervalSeconds, Settings::Morse settings
	);
	
	static char PulsesToChar(
		const Component::Morse::Transceiver::PulseArray& pulses, uint8_t pulseCount
	);

};