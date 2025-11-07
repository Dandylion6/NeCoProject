#pragma once
#include "raylib.h"
#include <functional>
#include <utility>


namespace Component::Action
{
/**
 * @brief Action component that defines key input for an entity.
 * 
 * Uses raylib's input API to listen for the specified `key` variable,
 * it only supports keyboard input. Once input is detected, the assigned
 * `onPressed` callback is called.
 * 
 * Usage example:
 * 
 * ```cpp
 * Component::Action::Inpit& input = registry.emplace<Component::Action::Input>(entity);
 * input.
 * ```
 */
struct Input 
{
    // ------ Members ------

    // TODO: Replace function with EnTT delegate.
    // TODO: Add more functionality, like onReleased.
    std::function<void()> onPressed { };
    KeyboardKey key = KEY_NULL;


    // ------ Constructors ------

    Input(KeyboardKey key) noexcept : key(key) { };
    Input(
        std::function<void()>&& onPressed,
        KeyboardKey key
    ) noexcept : 
        onPressed(std::move(onPressed)),
        key(key)
    { };
};

}