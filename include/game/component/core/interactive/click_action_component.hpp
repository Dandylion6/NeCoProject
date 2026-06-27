#pragma once
#include <cstdint>

#include "core/data/vector2.hpp"


namespace Component::Action
{
/**
 * @brief Action Component that defines an entity click action.
 *
 * <c>justClicked</c> is true on the frame the click action is performed.
 * <c>state</c> determines if the action can or can't be done.
 * If the click component cannot be identified by unique or relevant components alone an <c>id</c> can be used.
 *
 * Usage example:
 * @code
 * enum MenuButtons : uint16_t
 * {
 *		Play,
 *		Settings,
 *		Quit,
 * }
 * ...
 * registry.emplace<Component::Action::Click>(MenuButtons::Play);
 * @endcode
 */
struct Click final
{
    // ------ Types ------
    enum State : uint8_t
    {
        Active,
        Inactive,
        Disabled,
    };


    // ------ Members ------

    Nc::Vector2f offset = Nc::Vector2f::Zero();
    uint16_t id = 0;
    State state = Active;
    bool justClicked = false;
    bool justReleased = false;
    bool isHeld = false;


    // ------ Constructors ------

    constexpr Click() noexcept = default;
    constexpr explicit Click(const Nc::Vector2f offset) noexcept : offset(offset) {}


    constexpr explicit Click(
        const int16_t id,
        const Nc::Vector2f offset = Nc::Vector2f::Zero()
    ) noexcept : offset(offset), id(id) {}
};
}
