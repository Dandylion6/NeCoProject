#pragma once
namespace Component {
struct Transform;
}


struct SystemContext;


namespace System::Journal
{

class Turn final
{
public:
    static void Update(const SystemContext& context) noexcept;

private:
    static void PlayTurnSound(const SystemContext& context, const Component::Transform& transform) noexcept;

};

}
