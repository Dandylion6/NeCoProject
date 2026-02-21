#pragma once


struct SystemContext;


namespace System::Radar
{

class ScreenGlitch final
{
public:
    static void Update(const SystemContext& context);
    static void StartGlitch(const SystemContext& context, float intensity);

};

}
