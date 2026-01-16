#pragma once


namespace Component::Debug
{
/**
 * @brief Developer-controlled runtime settings.
 *
 * Stored in the ECS registry context.
 * These values may alter game behavior during development
 * and are ignored in release builds.
 *
 * Written on game initialization based on boot parameters.
 * 
 * Usage example:
 * @code
 * Component::Debug::DevSettings& devSettings = registry.emplace<Component::Debug::DevSettings>(entity);
 * 
 * devSettings.ignoreMainMenu = true;
 * devSettings.isMinimizedWindowed = true;
 * devSettings.isRadarActiveOnStart = true;
 * @endcode
 */
struct DevSettings final
{
	bool ignoreMainMenu = false;
	bool isMaximizedWindowed = false;
	bool isRadarActiveOnStart = false;
};

}
