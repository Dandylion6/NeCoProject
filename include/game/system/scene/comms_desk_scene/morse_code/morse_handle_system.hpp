#pragma once
struct SystemContext;


namespace System::Morse
{

class Handle final
{
public:
    static void Update(const SystemContext& context) noexcept;

};

}
