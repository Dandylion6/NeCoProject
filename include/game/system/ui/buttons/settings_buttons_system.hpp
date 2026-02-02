#pragma once


struct Settings;
struct SystemContext;


namespace System::Settings
{

class Buttons final
{
public:
    static void Update(const SystemContext& context, ::Settings& live, ::Settings& pending) noexcept;

private:
    static void ToMainMenu(const SystemContext& context, const ::Settings& live, ::Settings& pending) noexcept;
    static void ApplySettings(::Settings& live, const ::Settings& pending) noexcept;

};

}
