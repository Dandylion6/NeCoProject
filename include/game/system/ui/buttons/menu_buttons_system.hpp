#pragma once


struct SystemContext;


namespace System::Menu
{

class Buttons final
{
public:
    static void Update(const SystemContext& context);

private:
    static void Play(const SystemContext& context);
    static void ToSettings(const SystemContext& context);
    static void Exit(const SystemContext& context);

};

}
