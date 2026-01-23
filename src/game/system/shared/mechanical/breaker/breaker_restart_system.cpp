#include "game/system/shared/mechanical/breaker/breaker_restart_system.hpp"

#include "entt/entity/registry.hpp"
#include "game/component/core/interactive/toggle_component.hpp"
#include "game/component/shared/mechanical/breaker_component.hpp"
#include "game/component/shared/mechanical/machine_component.hpp"
#include "game/contexts/system_context.hpp"
#include "game/state/anomaly_state.hpp"


void System::Logic::Breaker::Restart::Update(const SystemContext& context, AnomalyState& anomaly) noexcept
{
    const auto view = context.registry.view<Component::Action::Toggle, Component::Logic::Breaker>();
    for (auto [entity, toggle, breaker] : view.each())
    {
        if (!context.registry.any_of<Component::Action::Toggle>(breaker.system)) continue;
        const auto& systemToggle = context.registry.get<Component::Action::Toggle>(breaker.system);

        if (systemToggle.state != Disabled) continue;

        const bool isReady = breaker.cycleTimerSeconds >= Component::Logic::Breaker::BREAKER_DISCHARGE_SECONDS;
        auto breakerContext = Context(breaker, toggle, anomaly, isReady);

        switch (toggle.state)
        {
        case Off: UpdateDischarge(context, breakerContext); break;
        default: UpdateBreakerOff(context, breakerContext); break;
        }
    }
}


void System::Logic::Breaker::Restart::UpdateDischarge(const SystemContext& context, const Context& breakerContext) noexcept
{
    constexpr float BREAKER_RESTART_WINDOW_SECONDS = 1.0f;
    constexpr float BREAKER_CYCLE_RESET_SECONDS = Component::Logic::Breaker::BREAKER_DISCHARGE_SECONDS + BREAKER_RESTART_WINDOW_SECONDS;

    breakerContext.breaker.cycleTimerSeconds = std::fmod(breakerContext.breaker.cycleTimerSeconds + context.deltaTime, BREAKER_CYCLE_RESET_SECONDS);
    breakerContext.breaker.status = breakerContext.isReady ? Component::Logic::Breaker::ReadyToRestart : Component::Logic::Breaker::Discharging;
}


void System::Logic::Breaker::Restart::UpdateBreakerOff(const SystemContext& context, const Context& breakerContext) noexcept
{
    if (breakerContext.isReady)
    {
        breakerContext.breaker.status = Component::Logic::Breaker::Operational;
        breakerContext.toggle.state = On;

        if (context.registry.all_of<Component::Action::Toggle>(breakerContext.breaker.system))
        {
            auto& systemToggle = context.registry.get<Component::Action::Toggle>(breakerContext.breaker.system);
            systemToggle.state = On;
        }

        breakerContext.breaker.cycleTimerSeconds = 0.0f;
        breakerContext.breaker.onRestart(context.registry, breakerContext.breaker.system);
        return;
    }

    // Starts the warning once.
    if (breakerContext.breaker.status != Component::Logic::Breaker::DesyncedRestart)
        DesyncRestart(context, breakerContext);

    breakerContext.breaker.desyncWarningSecondsLeft -= context.deltaTime;
    if (breakerContext.breaker.desyncWarningSecondsLeft <= 0.0f)
        breakerContext.breaker.status = Component::Logic::Breaker::Faulted;
}


void System::Logic::Breaker::Restart::DesyncRestart(const SystemContext& context, const Context& breakerContext) noexcept
{
    constexpr float DESYNC_WARNING_SECONDS = 0.96f;
    constexpr float ATTRACTION_GAIN_MULTIPLIER = 2.0f;

    breakerContext.breaker.status = Component::Logic::Breaker::DesyncedRestart;
    breakerContext.breaker.cycleTimerSeconds = 0.0f;
    breakerContext.breaker.desyncWarningSecondsLeft = DESYNC_WARNING_SECONDS;

    if (!context.registry.any_of<Component::Machine>(breakerContext.breaker.system)) return;

    const Component::Machine& machine = context.registry.get<Component::Machine>(breakerContext.breaker.system);
    breakerContext.anomaly.attractionPercentage += machine.attractionReduction * ATTRACTION_GAIN_MULTIPLIER;
}
