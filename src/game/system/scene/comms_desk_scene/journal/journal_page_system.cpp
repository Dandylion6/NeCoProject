#include "game/system/scene/comms_desk_scene/journal/journal_page_system.hpp"
#include <format>
#include "core/runtime/entity_helpers.hpp"
#include "core/runtime/resource_store.hpp"
#include "entt/entity/entity.hpp"
#include "game/component/core/rendering/sprite_component.hpp"
#include "game/component/scene/comms_desk_scene/journal_component.hpp"
#include "game/contexts/system_context.hpp"
#include "game/state/game_state.hpp"
#include "game/system/core/audio/audio_emitter_system.hpp"


void System::JournalPage::Update(const SystemContext& context) noexcept
{
    const entt::entity entity = entt::get_single<Component::Journal>(context.registry);
    auto& sprite = context.registry.get<Component::Sprite>(entity);
    const auto& journal = context.registry.get<Component::Journal>(entity);

    const Component::Journal::Page page = journal.pages.at(journal.index);
    const std::string path = PAGE_DIRECTORY + GetPagePath(page, journal.index);

    const Texture2D& pageAlbedoTexture = context.store.GetTexture(path);
    sprite.albedo = pageAlbedoTexture;
}


void System::JournalPage::OnDayChange(const SystemContext& context) noexcept
{

}


std::string System::JournalPage::GetPagePath(const Component::Journal::Page page, uint8_t index) noexcept
{
    switch (page.type)
    {
    case Component::Journal::Page::NotFound:
        break;
    case Component::Journal::Page::Found:
        return std::format("page_{}.png", index);
    case Component::Journal::Page::Partial:
        return std::format("page_{}_p.png", index);
    }
    return "page_n.png";
}
