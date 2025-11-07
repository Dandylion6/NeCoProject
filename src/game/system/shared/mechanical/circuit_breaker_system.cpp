#include "game/component/core/rendering/rectangle_component.hpp"
#include "game/component/shared/mechanical/circuit_breaker_component.hpp"
#include "game/component/core/interactive/toggle_component.hpp"
#include "game/component/shared/mechanical/machine_component.hpp"
#include "game/state/anomaly_state.hpp"
#include "entt/entity/registry.hpp"
#include "raylib.h"
#include "game/system/shared/mechanical/circuit_breaker_system.hpp"
#include "core/data/color.hpp"


void CircuitBreakerSystem::Update(
    entt::registry& registry, AnomalyState& anomalyState, float deltaTime
) noexcept
{
    auto view = registry.view<Component::Action::Toggle, Component::CircuitBreaker>();
    for (auto [entity, breakerToggle, breaker] : view.each())
    {
        bool systemHasToggle = registry.any_of<Component::Action::Toggle>(breaker.system);
        if (!systemHasToggle) continue;
        
        Component::Action::Toggle& systemToggle = registry.get<Component::Action::Toggle>(breaker.system);
        
        bool systemIsBroken = systemToggle.state == Disabled;
        if (systemIsBroken)
        {
            bool isReadyToRestart = breaker.cycleTimerSeconds >= Component::CircuitBreaker::BREAKER_DISCHARGE_SECONDS;
            bool isDischarging = breakerToggle.state == Off;
            if (isDischarging) 
                UpdateDischarge(breaker, breakerToggle, deltaTime, isReadyToRestart);
            else
                UpdateBreakerOff(registry, anomalyState, breaker, breakerToggle, systemToggle, isReadyToRestart, deltaTime);
        }
        else
        {
            UpdateNormalOperations(breaker, breakerToggle, systemToggle);
        }
        UpdateIndicator(registry, breaker.indicator, breaker, deltaTime);
    }
}


void CircuitBreakerSystem::UpdateDischarge(
    Component::CircuitBreaker& breaker, 
    Component::Action::Toggle& breakerToggle,
    float deltaTime,
    bool isReadyToRestart
) noexcept
{
    constexpr float BREAKER_RESTART_WINDOW_SECONDS = 1.0f;
    constexpr float BREAKER_CYCLE_RESET_SECONDS = Component::CircuitBreaker::BREAKER_DISCHARGE_SECONDS + BREAKER_RESTART_WINDOW_SECONDS;

    breaker.cycleTimerSeconds = std::fmodf(breaker.cycleTimerSeconds + deltaTime, BREAKER_CYCLE_RESET_SECONDS);
    if (isReadyToRestart)
        breaker.status = Component::CircuitBreaker::ReadyToRestart;
    else
        breaker.status = Component::CircuitBreaker::Discharging;
}


void CircuitBreakerSystem::UpdateBreakerOff(
    entt::registry& registry,
    AnomalyState& anomalyState, 
    Component::CircuitBreaker& breaker, 
    Component::Action::Toggle& breakerToggle, 
    Component::Action::Toggle& systemToggle,
    bool isReadyToRestart,
    float deltaTime
) noexcept
{
    
    if (isReadyToRestart)
    {
        breaker.status = Component::CircuitBreaker::Operational;
        breakerToggle.state = On;
        systemToggle.state = On;
        breaker.cycleTimerSeconds = 0.0f;
        breaker.onRestart(registry, breaker.system);
        return;
    }
    
    bool hasDesyncedRestart = breaker.cycleTimerSeconds > 0.0f;
    bool isIdleWarning = !hasDesyncedRestart && breaker.desyncWarningSecondsLeft <= 0.0f;
    if (isIdleWarning)
    {
        breaker.status = Component::CircuitBreaker::Faulted;
        return;
    }

    breaker.desyncWarningSecondsLeft -= deltaTime;

    if (hasDesyncedRestart)
    {
        DesyncRestart(registry, anomalyState, breaker);
        return;
    }
}


void CircuitBreakerSystem::DesyncRestart(
    entt::registry& registry, 
    AnomalyState& anomalyState, 
    Component::CircuitBreaker& breaker
) noexcept
{
    constexpr float DESYNC_WARNING_SECONDS = 0.96f;
    constexpr float ATTRACTION_GAIN_MULTIPLIER = 2.0f;

    breaker.status = Component::CircuitBreaker::DesyncedRestart;
    breaker.cycleTimerSeconds = 0.0f;
    breaker.desyncWarningSecondsLeft = DESYNC_WARNING_SECONDS;

    bool systemIsMachine = registry.any_of<Component::Machine>(breaker.system);
    if (systemIsMachine)
    {
        Component::Machine& machine = registry.get<Component::Machine>(breaker.system);
        anomalyState.attractionPercentage += machine.attractionReduction * ATTRACTION_GAIN_MULTIPLIER;
    }
}


void CircuitBreakerSystem::UpdateNormalOperations(
    Component::CircuitBreaker& breaker, 
    const Component::Action::Toggle& breakerToggle, 
    Component::Action::Toggle& systemToggle
) noexcept
{
    bool powerIsCut = breakerToggle.state == Off;
    bool forceDeactivation = powerIsCut && systemToggle.state == On;
    if (forceDeactivation)
        systemToggle.state = Off;

    switch (systemToggle.state)
    {
    case On:
        breaker.status = Component::CircuitBreaker::Operational;
        break;
    case Off:
        breaker.status = Component::CircuitBreaker::Offline;
        break;
    default:
        break;
    }
}


void CircuitBreakerSystem::UpdateIndicator(
    entt::registry& registry, 
    const entt::entity entity, 
    const Component::CircuitBreaker& breaker, 
    float deltaTime
) noexcept
{
    // TODO: Replace placeholder visuals.
    Component::Rectangle& rectangle = registry.get<Component::Rectangle>(entity);

    switch (breaker.status)
    {
    case Component::CircuitBreaker::Operational:
        rectangle.fillColor = GREEN;
        break;
    case Component::CircuitBreaker::Offline:
        rectangle.fillColor = WHITE;
        break;
    case Component::CircuitBreaker::Faulted:
        rectangle.fillColor = RED;
        break;
    case Component::CircuitBreaker::Discharging:
        rectangle.fillColor = WHITE;
        break;
    case Component::CircuitBreaker::ReadyToRestart:
        rectangle.fillColor = ORANGE;
        break;
    case Component::CircuitBreaker::DesyncedRestart:
        rectangle.fillColor = PINK;
        break;
    default:
        break;
    }
}