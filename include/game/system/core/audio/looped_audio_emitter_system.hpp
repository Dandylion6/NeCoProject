#pragma once


namespace Component {
struct LoopedAudio;
}


struct SystemContext;


namespace System::Audio
{

class LoopedEmitter final
{
public:
    // ------ Functions ------
    static void Update(const SystemContext& context);

private:
    static bool CanStop(const Component::LoopedAudio& audio);
    static bool CanPlay(const Component::LoopedAudio& audio);

};

}
