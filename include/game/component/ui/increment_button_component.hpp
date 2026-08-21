#pragma once
#include "entt/entity/entity.hpp"


namespace Component::UI
{
/**
 * @brief Defines a button for incrementing a value.
 *
 * Holds a reference to the <i>entity</i> that has the <c>Component::UI::Increment</c> component.
 * The button either increases or decreases the value, specified by the <c>Type</c>.
 *
 * Usage example:
 * @code
 * registry.emplace<Component::UI::IncrementButton>(entity, Component::UI::IncrementButton::Increase);
 * @endcode
 */
struct IncrementButton final
{
    // ------ Types ------
    enum Type : uint8_t
    {
        Decrease,
        Increase,
    };


    // ------ Members ------

    entt::entity increment;
    Type type;


    // ------ Constructors ------

    IncrementButton(const entt::entity entity, const Type type) : increment(entity), type(type) {}
};

}
