#pragma once
struct SystemContext;


namespace System::Artillery
{

class Aiming final
{
public:
	// ------ Functions ------
	static void Update(const SystemContext& context) noexcept;

};

}
