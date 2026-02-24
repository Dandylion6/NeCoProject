#include "game/system/scene/outside_scene/receiver/interpret_fire_system.hpp"

#include <string>

#include "raylib.h"
#include "core/runtime/entity_helpers.hpp"
#include "core/runtime/resource_store.hpp"
#include "entt/entity/fwd.hpp"
#include "entt/entity/registry.hpp"
#include "game/component/core/audio/sound_emitter_component.hpp"
#include "game/component/scene/comms_scene/radio_component.hpp"
#include "game/component/scene/outside_scene/artillery_component.hpp"
#include "game/component/scene/outside_scene/receiver_component.hpp"
#include "game/construction/scene/outside_scene/entity/projectile_entity.hpp"
#include "game/contexts/scene_context.hpp"
#include "game/contexts/system_context.hpp"
#include "game/system/core/audio/audio_emitter_system.hpp"
#include "game/system/scene/comms_scene/radio/radio_emitter_system.hpp"


void System::Receiver::Interpret::Fire::HandleMessage(const SystemContext& context, Component::Receiver& receiver)
{
	constexpr char FIRE_RESPONSE[] = "assets/audio/voicelines/receiver/commands/fire_request.wav";
	constexpr float FIRE_DELAY = 4.6f;

	//TODO: Add audio response for when not ready.
	const entt::entity entity = entt::get_single<Component::Artillery>(context.registry);
	auto& artillery = context.registry.get<Component::Artillery>(entity);

	artillery.receivedFireRequest = true;
	artillery.fireDelaySeconds = FIRE_DELAY;

	const Sound response = context.store.CreateSoundHandle(FIRE_RESPONSE);
	Radio::Emitter::Broadcast(context.registry, response, BroadcastPriority::Medium);

	receiver.currentContext = OnStandby;
	receiver.message.clear();
};


void System::Receiver::Interpret::Fire::Update(const SystemContext& context)
{
	const entt::entity entity = entt::get_single<Component::Artillery>(context.registry);
	auto& artillery = context.registry.get<Component::Artillery>(entity);
	auto& emitter = context.registry.get<Component::Audio>(entity);

	if (!artillery.receivedFireRequest) return;
	if (!artillery.isReadyToFire) return;

	if (artillery.fireDelaySeconds > 0.0f)
	{
		artillery.fireDelaySeconds -= context.deltaTime;
		return;
	}

	artillery.receivedFireRequest = false;
	Audio::Emitter::PlayEmitter(emitter);

	const auto sceneContext = SceneContext(context.registry, context.store, context.game);
	Entity::Projectile::Create(sceneContext, artillery.aimPosition);
};