#pragma once
#include <string>

#include "game/component/scene/comms_desk_scene/journal_component.hpp"
#include "game/system/core/audio/audio_emitter_system.hpp"
struct SystemContext;


namespace System
{

class JournalPage final
{
public:
    // ------ Members ------

    static constexpr char PAGE_DIRECTORY[] = "assets/environment/objects/note/";


    // ------ Functions ------

    static void Update(const SystemContext& context) noexcept;
    static void OnDayChange(const SystemContext& context) noexcept;

private:
    // ------ Functions ------

    static std::string GetPagePath(Component::Journal::Page page, uint8_t index) noexcept;

};

}
