#pragma once
#include "blip/glitch/blip_distortion_system.hpp"


namespace Component::Action {
struct Click;
}


struct SystemContext;


namespace System::Radar
{

class Buttons final
{
public:
    // ------ Functions ------
    static void Update(const SystemContext& context);

private:
    // ------ Members ------
    static constexpr auto BUTTON_OFFSET = Nc::Vector2f::Scale(1.0f);


    // ------ Functions ------

    static void Toggle(const SystemContext& context);

};

}
