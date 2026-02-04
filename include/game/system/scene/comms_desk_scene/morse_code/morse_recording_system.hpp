#pragma once
#include <cstdint>

#include "game/state/settings.hpp"
struct AnomalyState;
struct SystemContext;


namespace Component::Morse
{
struct Transceiver;
}


namespace System::Morse
{
class Recording final
{
public:
    // ------ Functions ------
    static void Update(const SystemContext& context, Settings::Morse settings) noexcept;

private:
    // ------ Types ------
    enum Type
    {
        Invalid,
        Short,
        Long,
    };


    // ------ Functions ------

    static void TryRecordPulse(Component::Morse::Transceiver& transceiver, float dotSeconds) noexcept;
    static void TryTransmitCharacter(const SystemContext& context, uint16_t decodingIndex) noexcept;
    static Type GetType(float intervalSeconds, float dotSeconds) noexcept;
};
}
