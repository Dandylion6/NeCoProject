#pragma once
#include "entt/entity/entity.hpp"


namespace Component::UI
{
/**
 * @brief Defines a button for toggling a value.
 *
 * Holds a reference to the <i>entity</i> that has <c>Component::Action::Toggle</c> component.
 * The button either enables or disables a value.
 *
 * Usage example:
 * @code
 * registry.emplace<Component::UI::ToggleButton>(entity, settingEntity);
 * @endcode
 */
struct ToggleButton final
{
    // ------ Members ------
    entt::entity toggle;


    // ------ Constructors ------

    explicit ToggleButton(const entt::entity entity) : toggle(entity) {}
};

}
