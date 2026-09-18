#include "game/system/scene/comms_desk_scene/journal/journal_turn_system.hpp"
#include "core/math/nc_math.hpp"
#include "core/runtime/entity_helpers.hpp"
#include "entt/entity/registry.hpp"
#include "game/component/core/interactive/drag_action_component.hpp"
#include "game/component/scene/comms_desk_scene/journal_component.hpp"
#include "game/contexts/system_context.hpp"
#include "game/system/scene/comms_desk_scene/journal/journal_page_system.hpp"


void System::JournalTurn::Update(const SystemContext& context) noexcept
{
    constexpr auto TURN_THRESHOLD = 20.0f;
    constexpr auto MAX_INDEX = static_cast<int>(Component::Journal::MAX_PAGES) - 1;

    const entt::entity entity = entt::get_single<Component::Journal>(context.registry);
    auto& journal = context.registry.get<Component::Journal>(entity);
    const auto& drag = context.registry.get<Component::Action::Drag>(entity);

    if (!drag.isTarget)
    {
        journal.pageTurned = false;
        return;
    }
    if (journal.pageTurned) return;
    if (std::fabsf(drag.draggedDelta.x) < TURN_THRESHOLD) return;

    journal.pageTurned = true;
    JournalPage::Update(context);

    const int sign = drag.draggedDelta.x > Nc::Math::EPSILON ? 1 : -1;
    const int nextPage = std::clamp(journal.index + sign, 0, MAX_INDEX);
    journal.index = nextPage;
}
