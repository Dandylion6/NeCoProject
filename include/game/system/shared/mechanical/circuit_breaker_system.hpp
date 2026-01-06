#pragma once
#include "game/component/shared/mechanical/circuit_breaker_component.hpp"
#include "game/component/core/interactive/toggle_component.hpp"
#include "entt/entity/fwd.hpp"
struct AnomalyState;


struct CircuitBreakerSystem
{
public:
    static void Update(
        entt::registry& registry,
        AnomalyState& anomalyState,
        float deltaTime
    ) noexcept;

private:
    static void UpdateDischarge(
        Component::Logic::CircuitBreaker& breaker,
        Component::Action::Toggle& breakerToggle,
        float deltaTime,
        bool isReadyToRestart
    ) noexcept;

    static void UpdateBreakerOff(
        entt::registry& registry,
        AnomalyState& anomalyState,
        Component::Logic::CircuitBreaker& breaker,
        Component::Action::Toggle& breakerToggle,
        Component::Action::Toggle& systemToggle,
        bool isReadyToRestart,
        float deltaTime
    ) noexcept;

    static void DesyncRestart(
        entt::registry& registry,
        AnomalyState& anomalyState,
        Component::Logic::CircuitBreaker& breaker
    ) noexcept;

    static void UpdateNormalOperations(
        Component::Logic::CircuitBreaker& breaker,
        const Component::Action::Toggle& breakerToggle,
        Component::Action::Toggle& systemToggle
    ) noexcept;

    static void UpdateIndicator(
        entt::registry& registry, 
        entt::entity entity,
        const Component::Logic::CircuitBreaker& breaker,
        float deltaTime
    ) noexcept;

};
