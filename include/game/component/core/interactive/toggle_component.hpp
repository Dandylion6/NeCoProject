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
 * The component's <c>state</c> can either be `Disabled</c>, <c>Off</c< or <c>On</c>.
 * This is used by systems to create toggleable behavior.
 * 
 * Usage example:
 * @code
 * registry.emplace<Component::Action::Toggle>(entity, On);
 * registry.emplace<Component::Machine>(entity, ...);
 * @endcode
 */
struct Toggle final
{
    // ------ Members ------

    ToggleState state = On;


    // ------ Constructors ------

    constexpr Toggle() noexcept = default;
    explicit constexpr Toggle(const ToggleState state) noexcept : state(state) { }


    // ------ Utility ------

    /**
    * @brief Toggles based on state given.
    * 
    * @param state Is the state to change based on toggle behavior.
    * @return The next logical state.
    */
    static constexpr ToggleState Next(const ToggleState state) noexcept
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