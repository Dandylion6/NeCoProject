#pragma once
#include <cstdint>


enum ToggleState : int8_t
{
    Disabled = -1,
    Off = 0,
    On = 1,
};


namespace Component
{
    struct Toggle
    {
        ToggleState state = On;

        Toggle() = default;
        Toggle(ToggleState state) : state(state) { };

        /// @brief Toggles based on state given.
        /// @param state Is the state to change based on toggle behaviour.
        /// @return 
        static ToggleState SetToggle(ToggleState state)
        {
            switch (state)
            {
            case Disabled: return Disabled;
            case Off: return On;
            case On: return Off;
            }
            return Disabled;
        }
    };
}


namespace ToggleLogic
{
    /// @brief Toggles based on state given.
    /// @param state Is the state to change based on toggle behaviour.
    /// @return The next logical state.
    static ToggleState Next(ToggleState state)
    {
        switch (state)
        {
        case Disabled: return Disabled;
        case Off: return On;
        case On: return Off;
        }
        return state;
    }
}