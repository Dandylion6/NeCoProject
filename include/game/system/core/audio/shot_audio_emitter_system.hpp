#pragma once
#include "game/system/scene/comms_scene/radar/blip/glitch/blip_distortion_system.hpp"


namespace Component {
struct ShotAudio;
}


struct SystemContext;


namespace System::Audio
{

class ShotEmitter final
{
public:
    // ------ Functions ------
    static void Update(const SystemContext& context);

private:
    static bool CanStop(const Component::ShotAudio& audio);
    static bool CanPlay(const Component::ShotAudio& audio);

};

}
