#pragma once
#include "components/objects/interactions/circuit_breaker_component.hpp"
#include "components/objects/interactions/toggle_component.hpp"
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
        Component::CircuitBreaker& breaker,
        Component::Toggle& breakerToggle,
        float deltaTime,
        bool isReadyToRestart
    ) noexcept;

    static void UpdateBreakerOff(
        entt::registry& registry,
        AnomalyState& anomalyState,
        Component::CircuitBreaker& breaker,
        Component::Toggle& breakerToggle,
        Component::Toggle& systemToggle,
        bool isReadyToRestart,
        float deltaTime
    ) noexcept;

    static void DesyncRestart(
        entt::registry& registry,
        AnomalyState& anomalyState,
        Component::CircuitBreaker& breaker
    ) noexcept;

    static void UpdateNormalOperations(
        Component::CircuitBreaker& breaker,
        const Component::Toggle& breakerToggle,
        Component::Toggle& systemToggle
    ) noexcept;

    static void UpdateIndicator(
        entt::registry& registry, 
        const entt::entity entity,
        const Component::CircuitBreaker& breaker,
        float deltaTime
    ) noexcept;

};
