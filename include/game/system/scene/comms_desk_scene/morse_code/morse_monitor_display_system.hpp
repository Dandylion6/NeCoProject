#pragma once
#include "game/state/settings.hpp"


struct SystemContext;


namespace System::Morse
{

class MonitorDisplay final
{
public:
	// ------ Functions ------
	static void Update(const SystemContext& context, Settings::Morse settings);

};

}