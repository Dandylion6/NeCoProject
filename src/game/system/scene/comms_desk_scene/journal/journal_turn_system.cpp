#include "game/system/scene/comms_desk_scene/journal/journal_turn_system.hpp"
#include "core/math/nc_math.hpp"
#include "core/math/random.hpp"
#include "core/runtime/entity_helpers.hpp"
#include "core/runtime/resource_store.hpp"
#include "entt/entity/registry.hpp"
#include "game/component/core/interactive/drag_action_component.hpp"
#include "game/component/scene/comms_desk_scene/journal_component.hpp"
#include "game/construction/scene/outside_scene/entity/artillery_entity.hpp"
#include "game/construction/shared/entity/environment/runtime_audio_entity.hpp"
#include "game/contexts/system_context.hpp"
#include "game/system/scene/comms_desk_scene/journal/journal_page_system.hpp"


void System::Journal::Turn::Update(const SystemContext& context) noexcept
{
    constexpr auto TURN_THRESHOLD = 100.0f;
    constexpr auto LAST_PAGE = static_cast<int>(Component::Journal::MAX_PAGES) - 1;

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

    const int direction = drag.draggedDelta.x > Nc::Math::EPSILON ? -1 : 1;
    const int nextPage = std::clamp(journal.index + direction, 0, LAST_PAGE);
    if (nextPage == journal.index) return;

    journal.index = nextPage;

    PlayTurnSound(context, context.registry.get<Component::Transform>(entity));
    Page::Update(context.registry, context.store);
}


void System::Journal::Turn::PlayTurnSound(const SystemContext& context, const Component::Transform& transform) noexcept
{
    constexpr auto VOLUME_RANGE = Nc::Vector2f(0.7f, 0.9f);
    constexpr std::array<std::string_view, 3> TURN_SOUNDS = {
        "assets/audio/object/paper/turn_page_0.wav",
        "assets/audio/object/paper/turn_page_1.wav",
        "assets/audio/object/paper/turn_page_2.wav"
    };

    auto& randomService = context.registry.ctx().get<Nc::Random>();
    const int i = randomService.RangeInt(0, TURN_SOUNDS.size() - 1);
    const auto file = std::string(TURN_SOUNDS[i]);

    const float volume = randomService.RangeFloat(VOLUME_RANGE.x, VOLUME_RANGE.y);
    Entity::RuntimeAudio::Create(context.store, context.registry, file, transform, volume);
}
