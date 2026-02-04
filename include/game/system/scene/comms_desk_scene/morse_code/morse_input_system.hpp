#pragma once
struct AnomalyState;
struct SystemContext;


namespace System::Morse
{

class Input final
{
public:
    static void Update(const SystemContext& context, AnomalyState& anomaly) noexcept;

};

}
