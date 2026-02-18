#pragma once
#include "core/data/vector2.hpp"
#include "game/component/core/interactive/click_action_component.hpp"
#include "game/system/scene/comms_scene/radar/blip/glitch/blip_distortion_system.hpp"
struct SystemContext;


namespace Nc
{
struct RenderContext;
}


namespace System::Action
{
// TODO: Separate target with click logic.
class Click final
{
public:
	// ------ Functions ------
	static void Update(const SystemContext& context, const Nc::RenderContext& renderContext);

private:
    // ------ Types ------
    enum State
    {
        None,
        JustClicked,
        IsHeld,
        JustReleased,
    };


	// ------ Functions ------

    static void UpdateSceneButtons(const SystemContext& context, const Nc::RenderContext& renderContext, State state);
	static void UpdateUiButtons(const SystemContext& context, Nc::Vector2i windowSize, State state);
	static void UpdateUiButtonState(Component::Action::Click& button, bool isVisible);
    static State GetClickState();

};

}
