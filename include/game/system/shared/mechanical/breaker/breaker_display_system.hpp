#pragma once


struct SystemContext;


namespace System::Logic::Breaker
{

class Display final
{
public:
    // ------ Functions ------
    static void Update(const SystemContext& context) noexcept;

};

}
