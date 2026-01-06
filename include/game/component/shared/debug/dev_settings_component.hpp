#pragma once


namespace Component::Debug
{
/**
 * @brief Developer-controlled runtime settings.
 *
 * Stored in the ECS registry context.
 * These values may alter game behaviour during development
 * and are ignored in release builds.
 *
 * Written on game initialization based on boot parameters.
 */
struct DevSettings final
{
	bool ignoreMainMenu = false;
	bool isMaximizedWindowed = false;
	bool isRadarActiveOnStart = false;
};

}