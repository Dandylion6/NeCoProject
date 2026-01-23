#pragma once
#include "entt/entity/entity.hpp"
#include "entt/entity/fwd.hpp"
#include "entt/signal/delegate.hpp"


namespace Component::Logic
{
/**
 * @brief Represents a circuit breaker entity.
 * 
 * Holds variables relating to restarting a system and
 * its status. Relies on `Component::Logic::Lever`
 * 
 * Usage example:
 * @code
 * entt::entity systemEntity = registry.create();
 * ...
 * entt::entity indicatorEntity = registry.create();
 * ...
 * registry.emplace<Component::Logic::Lever>(entity, ...);
 * registry.emplace<Component::Logic::CircuitBreaker>(entity, systemEntity, indicatorEntity);
 * @endcode
 */
struct Breaker
{
    using OnRestart = entt::delegate<void(entt::registry&, entt::entity)>;
    static constexpr float BREAKER_DISCHARGE_SECONDS = 6.0f;

    enum Status : uint8_t
    {
        /**
         * @brief The system is running as intended.
         */
        Operational,
        /**
         * @brief The system has been manually set to offline.
         */
        Offline,
        /**
         * @brief The system brokedown and requires attention.
         */
        Faulted,
        /**
         * @brief Breaker has been set off and is waiting for restart.
         */
        Discharging,
        /**
         * @brief The breaker is ready to restart.
         */
        ReadyToRestart,
        /**
         * @brief Restart was miss-timed.
         */
        DesyncedRestart
    };


    // ------ Members ------

    OnRestart onRestart { };
    entt::entity system = entt::null;
    entt::entity indicator = entt::null;
    float cycleTimerSeconds = 0.0f;
    float desyncWarningSecondsLeft = 0.0f;
    Status status = Operational;


    // ------ Constructors ------

    constexpr Breaker(const entt::entity system, const entt::entity indicator) noexcept :
        system(system), indicator(indicator) { }
};   

}