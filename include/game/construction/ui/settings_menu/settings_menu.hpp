#pragma once
#include "core/data/vector2.hpp"
#include "game/component/core/interactive/toggle_component.hpp"
#include "game/contexts/scene_context.hpp"
struct Settings;


namespace Structure
{

class SettingsMenu final
{
public:
	// ------ Types ------
	enum ButtonId : uint16_t
	{
		ToMainMenu,
		ApplySettings,
	};


	// ------ Functions ------

	static void Build(
		const SceneContext& context,
		Nc::Vector2f windowSize,
		Settings& live,
		Settings& pending
	) noexcept;

	static void Toggle(const SceneContext& context) noexcept;
	static void Open(const SceneContext& context) noexcept;
	static void Close(const SceneContext& context) noexcept;
	static void Close(const SceneContext& context, const Settings& live, Settings& pending) noexcept;

private:
	// ------ Functions ------
	static void Toggle(const SceneContext& context, ToggleState state) noexcept;

};

}
