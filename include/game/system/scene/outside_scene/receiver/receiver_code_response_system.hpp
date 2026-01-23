#pragma once
struct SystemContext;


namespace Component
{
struct Receiver;
}


namespace Nc
{
class ResourceStore;
};


namespace System::Receiver
{

class CodeResponse final
{
public:
	// ------ Functions ------
	static void Update(const SystemContext& context) noexcept;

private:
	// ------ Functions ------
	static void ConfirmCodeReceived(const SystemContext& context, Component::Receiver& receiver);
};

}

