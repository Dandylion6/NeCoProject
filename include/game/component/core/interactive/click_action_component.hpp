#pragma once
#include <functional>
#include <utility>


namespace Component::Action
{
/**
 * @brief Action Component that defines an entity click action.
 *
 * Used for UI elements or interactive world objects that need to respond
 * to user clicks. When triggered, executes the assigned
 * `onClick` callback function.
 *
 * The component's `isActive` value turns false once clicked and must be 
 * manually set to true again.
 *
 * Usage example:
 * 
 * ```cpp
 * Component::Action::Click& click = registry.emplace<Component::Action::Click>();
 * click.onClick = []()
 * {
 *     std::cout << "Button clicked!\n";
 * };
 * ```
 */
struct Click final
{
	// ------ Members ------

	// TODO: Replace function with EnTT delegate.
	std::function<void()> onClick = std::function<void()>();
	bool isActive = true;


	// ------ Constructors ------

	constexpr Click() noexcept = default;
	Click(std::function<void()>&& onClick) noexcept : 
		onClick(std::move(onClick))
	{ };
};

}