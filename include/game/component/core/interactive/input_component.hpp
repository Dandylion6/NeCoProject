#pragma once
#include "raylib.h"


namespace Component::Action
{
/**
 * @brief Action component that defines key input for an entity.
 * 
 * Uses raylib's input API to listen for the specified <c>key</c>,
 * it only supports keyboard input. Values like <c>justPressed</c> and <c>justReleased</c>
 * are frame dependant.
 * 
 * Usage example:
 * @code
 * constexpr KeyboardKey KEY = KEY_A;
 * constexpr bool SCENE_BOUND_INPUT = false
 *
 * registry.emplace<Component::Action::Input>(entity, KEY, SCENE_BOUND_INPUT);
 * @endcode
 */
struct Input final
{
    // ------ Types ------
    enum State : uint8_t
    {
        Idle,
        Held,
        Pressed,
        Released,
    };


    // ------ Members ------

    KeyboardKey key = KEY_NULL;
    State state = Idle;
    bool isSceneBound = true;


    // ------ Constructors ------

    explicit Input(const KeyboardKey key, const bool isSceneBound = true) noexcept
        : key(key), isSceneBound(isSceneBound) {}
};
}
