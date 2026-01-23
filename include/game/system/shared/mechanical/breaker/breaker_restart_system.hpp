#pragma once

namespace Component::Action
{
struct Toggle;
}


namespace Component::Logic
{
struct Breaker;
}


struct AnomalyState;
struct SystemContext;


namespace System::Logic::Breaker
{
class Restart final
{
public:
    // ------ Functions ------
    static void Update(const SystemContext& context, AnomalyState& anomaly) noexcept;

private:
    // ------ Members ------
    struct Context final
    {
        Component::Logic::Breaker& breaker;
        Component::Action::Toggle& toggle;
        AnomalyState& anomaly;
        bool isReady = false;


        Context(
            Component::Logic::Breaker& breaker,
            Component::Action::Toggle& toggle,
            AnomalyState& anomaly,
            const bool isReady
        ) noexcept :
            breaker(breaker),
            toggle(toggle),
            anomaly(anomaly),
            isReady(isReady) { }
    };


    // ------ Functions ------

    static void UpdateDischarge(const SystemContext& context, const Context& breakerContext) noexcept;
    static void UpdateBreakerOff(const SystemContext& context, const Context& breakerContext) noexcept;
    static void DesyncRestart(const SystemContext& context, const Context& breakerContext) noexcept;
};
}
