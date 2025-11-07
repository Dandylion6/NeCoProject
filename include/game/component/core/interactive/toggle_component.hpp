#pragma once
#include <cstdint>


enum ToggleState : int8_t
{
    Disabled = -1,
    Off = 0,
    On = 1,
};


namespace Component::Action
{
/**
 * @brief Action component that defines an entities toggle state.
 * 
 * The component's `state` can either be `Disabled`, `Off` or `On`. 
 * This is used by systems to create toggleable behaviour.
 * 
 * Usage example:
 * 
 * ```cpp
 * registry.emplace<Component::Action::Toggle>(entity, On);
 * registry.emplace<Component::Machine>(entity, ...);
 * ```
 */
struct Toggle
{
    // ------ Members ------

    ToggleState state = On;


    // ------ Constructors ------

    constexpr Toggle() noexcept = default;
    constexpr Toggle(ToggleState state) noexcept : 
        state(state) 
    { };


    // ------ Utility ------

    /**
    * @brief Toggles based on state given.
    * 
    * @param state Is the state to change based on toggle behaviour.
    * @return The next logical state.
    */
    static constexpr ToggleState Next(
        ToggleState state
    ) noexcept
    {
        switch (state)
        {
        case Disabled: return Disabled;
        case Off: return On;
        case On: return Off;
        }
        return state;
    }
};

}