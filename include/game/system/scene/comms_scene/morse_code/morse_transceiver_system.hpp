#pragma once
#include <cstdint>

#include "entt/entity/fwd.hpp"
#include "game/component/scene/comms_scene/morse_components.hpp"
#include "game/state/settings.hpp"
#include "game/utility/morse_code.hpp"
struct SystemContext;
struct AnomalyState;


namespace System::Morse
{

class Transceiver final
{
public:
	// ------ Functions ------
	static void Update(const SystemContext& context, AnomalyState& anomaly, Settings::Morse settings);

private:
	// ------ Functions ------
	static void InputChanged(Component::Morse::Transceiver& transceiver, Settings::Morse settings);
	static void TryEndCharacter(
		entt::registry& registry,
		AnomalyState& anomalyState,
		Component::Morse::Transceiver& transceiver,
		Settings::Morse settings
	);

	static void TransmitCharacter(entt::registry& registry, AnomalyState& anomalyState, char character);
	static void RecordPulse(Component::Morse::Transceiver& transceiver, Settings::Morse settings);
	static void ClearTransceiver(Component::Morse::Transceiver& transceiver);
	static MorseCode::Pulse GetPulseType(float intervalSeconds, Settings::Morse settings);
	static char PulsesToChar(const Component::Morse::Transceiver::PulseArray& pulses, uint8_t pulseCount);

};

}