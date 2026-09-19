#pragma once
#include <string>
#include "entt/entity/registry.hpp"
#include "game/component/scene/comms_desk_scene/journal_component.hpp"
#include "game/system/core/audio/audio_emitter_system.hpp"


namespace Nc {
class ResourceStore;
}


struct SystemContext;


namespace System::Journal
{

class Page final
{
public:
    // ------ Members ------

    static constexpr char PAGE_DIRECTORY[] = "assets/environment/objects/journal/";


    // ------ Functions ------

    static void Update(entt::registry& registry, Nc::ResourceStore& store) noexcept;
    static void OnDayChange(const SystemContext& context) noexcept;

private:
    // ------ Functions ------

    static std::string GetPagePath(Component::Journal::Page page, uint8_t index) noexcept;

};

}
