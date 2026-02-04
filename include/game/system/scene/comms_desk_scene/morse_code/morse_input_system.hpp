#pragma once
struct SystemContext;


namespace System::Morse
{

class Input final
{
public:
    static void Update(const SystemContext& context) noexcept;

};

}
