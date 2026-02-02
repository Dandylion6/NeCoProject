#pragma once
struct SystemContext;


namespace System::Restart
{

class Buttons final
{
public:
    static void Update(const SystemContext& context) noexcept;

private:
    static void ToMainMenu(const SystemContext& context) noexcept;
    static void Restart(const SystemContext& context) noexcept;

};

}
