#pragma once
struct SystemContext;


namespace System::Journal
{

class Turn final
{
public:
    static void Update(const SystemContext& context) noexcept;

};

}
