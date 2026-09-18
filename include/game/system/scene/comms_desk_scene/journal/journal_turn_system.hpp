#pragma once
struct SystemContext;


namespace System
{

class JournalTurn final
{
public:
    static void Update(const SystemContext& context) noexcept;

};

}
