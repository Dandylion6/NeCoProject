#include "game/game.hpp"

#include <cmath>

#include "raylib.h"
#include "core/data/vector2.hpp"
#include "core/runtime/entity_helpers.hpp"
#include "core/runtime/render_context.hpp"
#include "entt/entity/fwd.hpp"
#include "game/construction/scene/comms_desk_scene/comms_desk_scene.hpp"
#include "game/construction/scene/comms_scene/comms_scene.hpp"
#include "game/construction/scene/doorway_scene/doorway_scene.hpp"
#include "game/construction/scene/outside_scene/outside_scene.hpp"
#include "game/construction/shared/entity/environment/ambient_sound_entity.hpp"
#include "game/construction/ui/main_menu/main_menu.hpp"
#include "game/construction/ui/restart_menu/restart_menu.hpp"
#include "game/construction/ui/settings_menu/settings_menu.hpp"
#include "game/construction/ui/shared/entity/move_transition_entity.hpp"
#include "game/contexts/build_context.hpp"
#include "game/contexts/states_context.hpp"
#include "game/contexts/system_context.hpp"
#include "game/save/save_settings.hpp"
#include "game/state/game_state.hpp"
#include "game/system/core/tween_system.hpp"
#include "game/system/core/audio/main_ambience_system.hpp"
#include "game/system/core/audio/audio_emitter_system.hpp"
#include "game/system/core/interactive/click_action_system.hpp"
#include "game/system/core/interactive/drag_action_system.hpp"
#include "game/system/core/interactive/input_action_system.hpp"
#include "game/system/core/rendering/rendering_system.hpp"
#include "game/system/core/rendering/lighting/lighting_system.hpp"
#include "game/system/scene/comms_scene/morse_code/morse_monitor_display_system.hpp"
#include "game/system/scene/comms_scene/morse_code/morse_tone_system.hpp"
#include "game/system/scene/comms_scene/morse_code/morse_transceiver_system.hpp"
#include "game/system/scene/comms_scene/radar/radar_artillery_system.hpp"
#include "game/system/scene/comms_scene/radar/radar_render_system.hpp"
#include "game/system/scene/comms_scene/radar/radar_stability_system.hpp"
#include "game/system/scene/comms_scene/radar/blip/blip_blink_system.hpp"
#include "game/system/scene/comms_scene/radar/blip/blip_death_system.hpp"
#include "game/system/scene/comms_scene/radar/blip/blip_glitch_system.hpp"
#include "game/system/scene/comms_scene/radio/radio_emitter_system.hpp"
#include "game/system/scene/outside_scene/artillery/artillery_aiming_system.hpp"
#include "game/system/scene/outside_scene/artillery/projectile_hit_system.hpp"
#include "game/system/scene/outside_scene/receiver/interpret_fire_system.hpp"
#include "game/system/scene/outside_scene/receiver/interpret_recalibration_system.hpp"
#include "game/system/scene/outside_scene/receiver/receiver_code_response_system.hpp"
#include "game/system/scene/outside_scene/receiver/receiver_command_processor_system.hpp"
#include "game/system/shared/anomaly/anomaly_attraction_system.hpp"
#include "game/system/shared/anomaly/roamer/roamer_behaviour_system.hpp"
#include "game/system/shared/anomaly/roamer/roamer_kill_system.hpp"
#include "game/system/shared/anomaly/roamer/roamer_spawning_system.hpp"
#include "game/system/shared/mechanical/circuit_breaker_system.hpp"
#include "game/system/shared/mechanical/lever_system.hpp"
#include "game/system/shared/mechanical/machine_power_system.hpp"
#include "game/system/shared/mechanical/breaker/breaker_display_system.hpp"
#include "game/system/shared/mechanical/breaker/breaker_operation_system.hpp"
#include "game/system/shared/mechanical/breaker/breaker_restart_system.hpp"
#include "game/system/ui/interactive/increment_value_system.hpp"
#include "game/utility/color_palette.hpp"

#ifdef DEBUG_BUILD
#include <cstring>
#include <string>

#include "game/component/shared/debug/dev_settings_component.hpp"
#include "game/component/shared/debug/runtime_readouts_component.hpp"
#include "game/save/save_game.hpp"
#include "game/state/scene.hpp"
#include "game/tag/core/life_cycle/dont_destroy_on_load_tag.hpp"
#include "game/utility/morse_code.hpp"
#endif // DEBUG_BUILD


Game::Game() : renderContext(resourceStore)
{
	InitAudioDevice();

	Load::SettingsFromDisk(settings);
	pendingSettings = settings;

	registry.ctx().emplace<Nc::Random>();
};


void Game::SetupRenderContext()
{
	constexpr Nc::Vector2i displaySize = Nc::RENDER_RESOLUTION;
	constexpr Nc::Vector2i radarSize = Nc::Vector2i(RADAR_BOUNDS.max);

	renderContext.renderTexture = LoadRenderTexture(displaySize.x, displaySize.y);
	renderContext.radarRenderTexture = LoadRenderTexture(radarSize.x, radarSize.y);

	const int monitor = GetCurrentMonitor();
	const Nc::Vector2i monitorSize = Nc::Vector2i(GetMonitorWidth(monitor), GetMonitorHeight(monitor));
	renderContext.windowSize = monitorSize;

	// Calculate render scale (preserve aspect ratio, clamp to nearest 0.1)
	const float scaleX = static_cast<float>(monitorSize.x) / static_cast<float>(displaySize.x);
	const float scaleY = static_cast<float>(monitorSize.y) / static_cast<float>(displaySize.y);
	const float scale = std::floor(std::fminf(scaleX, scaleY) * 10.0f) * 0.1f;
	renderContext.renderScale = scale;

	// Compute scaled display size and centered position
	const Nc::Vector2f scaledDisplay = Nc::Vector2f(displaySize) * scale;
	const Nc::Vector2f origin = (Nc::Vector2f(monitorSize) - scaledDisplay) * 0.5f;
	renderContext.renderRectangle = {origin.x, origin.y, scaledDisplay.x, scaledDisplay.y};

	LightingSystem::Initialize(renderContext.lightingContext, resourceStore);
}


#ifdef DEBUG_BUILD
void Game::SetupDebug(const int args, char* argv[])
{
	const entt::entity entity = registry.create();

	registry.emplace<Tag::DontDestroyOnLoad>(entity);
	registry.emplace<Component::Debug::RuntimeReadouts>(entity);

	auto& [ignoreMainMenu, isMaximizedWindowed, isRadarActiveOnStart] =
		registry.emplace<Component::Debug::DevSettings>(entity);
	for (int i = 0; i < args; ++i)
	{
		if (strcmp(argv[i], "--ignore-main-menu") == 0)
		{
			ignoreMainMenu = true;
			continue;
		}

		if (strcmp(argv[i], "--maximized-windowed") == 0)
		{
			isMaximizedWindowed = true;
			continue;
		}

		if (strcmp(argv[i], "--auto-start-radar") == 0)
		{
			isRadarActiveOnStart = true;
			continue;
		}

		if (strcmp(argv[i], "--start-at-night") == 0)
		{
			gameState.hour = GameState::NIGHT_RANGE.x;
		}
	}
}
#endif


void Game::Save()
{
	Save::GameToDisk(registry, StatesContext(anomalyState, gameState));
}


void Game::Load()
{
	// Clean up any entities that are load dependant.
	std::vector<entt::entity> toClean;
    for (entt::entity entity : registry.view<entt::entity>())
    {
	    if (!registry.any_of<Tag::DontDestroyOnLoad>(entity))
            toClean.emplace_back(entity);
    }
    for (const entt::entity entity : toClean) registry.destroy(entity);

    BuildRuntimeScenes();

	Load::GameFromDisk(registry, StatesContext(anomalyState, gameState));
}


void Game::BuildMenuUI()
{
	const Nc::Vector2f windowSize = Nc::Vector2f(renderContext.windowSize);
	const SceneContext context = SceneContext(registry, resourceStore, gameState);

	Structure::MainMenu::Build(context, *this);
	Structure::SettingsMenu::Build(context, windowSize, settings, pendingSettings);
	Structure::RestartMenu::Build(context, *this, windowSize);

#ifdef DEBUG_BUILD
	const entt::entity entity = entt::get_single<Component::Debug::DevSettings>(registry);
	const auto& devSettings = registry.get<Component::Debug::DevSettings>(entity);

	if (!devSettings.ignoreMainMenu) Structure::MainMenu::Open(context);
	else
	{
		// Load a mock game state for testing.
		gameState.currentScene = CommsRoom;
		BuildRuntimeScenes();
	}

#else
	MainMenu::Open(registry, gameState);
#endif // DEBUG_BUILD
}


void Game::BuildRuntimeScenes()
{
	const SceneContext sceneContext = SceneContext(registry, resourceStore, gameState);

	Entity::MoveTransition::Create(sceneContext, renderContext);
	Entity::AmbientSound::Create(registry);

	const BuildContext buildContext = BuildContext(registry, resourceStore, renderContext, gameState);

	Structure::CommsScene::Build(buildContext);
	Structure::DeskScene::Build(buildContext);
	Structure::DoorwayScene::Build(buildContext);
	Structure::OutsideScene::Build(buildContext);
}


void Game::SetupWindow()
{
	SetConfigFlags(FLAG_VSYNC_HINT);

	const auto monitorSize = Nc::Vector2i(GetMonitorWidth(0), GetMonitorHeight(0));
	InitWindow(monitorSize.x, monitorSize.y, "Negative Contact");

#ifdef DEBUG_BUILD
	SetExitKey(KEY_BACKSPACE);
	
	const entt::entity entity = entt::get_single<Component::Debug::DevSettings>(registry);
	const auto& devSettings = registry.get<Component::Debug::DevSettings>(entity);

	if (!devSettings.isMaximizedWindowed)
		SetWindowState(FLAG_FULLSCREEN_MODE);
	else
	{
		SetWindowState(FLAG_WINDOW_RESIZABLE);
		MaximizeWindow();
	}

#else
	SetExitKey(KEY_NULL);
	SetWindowState(FLAG_FULLSCREEN_MODE);
#endif
}


void Game::Shutdown()
{
	// TODO: Add safety features
	CloseAudioDevice();
	CloseWindow();
}


bool Game::ShouldRun() const
{
	if (WindowShouldClose()) return false;
	if (gameState.shouldExit) return false;
	return true;
}


void Game::Update(float deltaTime)
{
	constexpr float NIGHT_END_HOUR = GameState::NIGHT_RANGE.x + 1.0f;

#ifdef DEBUG_BUILD
	if (IsKeyPressed(KEY_PERIOD))
	{
		auto& randomService = registry.ctx().get<Nc::Random>();
		const Nc::Vector2f spawnPoint = RoamerSpawningSystem::GenerateRandomSpawnPoint(randomService);
		RoamerSpawningSystem::SpawnRoamer({registry, resourceStore, gameState, deltaTime}, spawnPoint, anomalyState);
	}
	
	if (IsKeyPressed(KEY_MINUS)) ++anomalyState.intensityLevel;
	if (IsKeyPressed(KEY_EQUAL)) --anomalyState.intensityLevel;
	
	if (IsKeyPressed(KEY_NINE)) anomalyState.attractionPercentage += 5.0f;
	if (IsKeyPressed(KEY_ZERO)) anomalyState.attractionPercentage -= 5.0f;

	const auto view = registry.view<Component::Debug::RuntimeReadouts>();
	for (auto [entity, readouts] : view.each())
	{
		if (IsKeyPressed(KEY_M)) readouts.timeScale += 0.5f;
		if (IsKeyPressed(KEY_N)) readouts.timeScale = std::fmaxf(readouts.timeScale - 0.5f, 0.0f);
		deltaTime *= readouts.timeScale;
	}

	if (IsKeyPressed(KEY_P)) Death({registry,resourceStore,gameState});
#endif

	// Reset cursor type.
	gameState.cursor = Nc::Cursor::Standard;

	if (gameState.isPaused) return;
	gameState.time += deltaTime;

	if (gameState.survivedNight) return;

	constexpr float HOUR_INCREASE_RATE = 1.0f / (GameState::HOUR_MINUTES * 60.0f);
	constexpr float HOURS_IN_DAY = 24.0f;

	const float oldHour = gameState.hour;
	const float newHour = std::fmod(gameState.hour + HOUR_INCREASE_RATE * deltaTime, HOURS_IN_DAY);
	gameState.hour = newHour;

	if (oldHour < NIGHT_END_HOUR && newHour >= NIGHT_END_HOUR)
		gameState.survivedNight = true;
}


void Game::UpdateRegistries(float deltaTime)
{
#ifdef DEBUG_BUILD
	const entt::entity debugEntity = entt::get_single<Component::Debug::RuntimeReadouts>(registry);
	const auto& readouts = registry.get<Component::Debug::RuntimeReadouts>(debugEntity);
	deltaTime *= readouts.timeScale;
#endif

	const auto context = SystemContext(registry, resourceStore, gameState, deltaTime);

	System::Action::Input::Update(context);
	System::Action::Click::Update(context, renderContext);
	System::Action::Drag::Update(context, renderContext);
	System::UI::IncrementValue::Update(registry);
	System::Audio::MainAmbience::Update(context);
	System::Tween::Update(context);
	System::Audio::Emitter::Update(context);

	MouseCursor cursor = MOUSE_CURSOR_DEFAULT;
	switch (gameState.cursor)
	{
	case Nc::Cursor::Clickable:
		cursor = MOUSE_CURSOR_POINTING_HAND;
		break;
	default: break;
	}
	SetMouseCursor(cursor);

	if (gameState.isPaused) return;

	System::Morse::Transceiver::Update(context, anomalyState, settings.morseSettings);
	System::Morse::MonitorDisplay::Update(context, settings.morseSettings);
	System::Morse::Tone::Update(context);
	System::Receiver::Interpret::Recalibration::Update(registry, deltaTime);
	System::Machine::PowerUsage::Update(context, anomalyState);
	System::Radar::Stability::Update(context, anomalyState);
	System::Radar::Artillery::Update(context);
	System::Blip::Death::Update(registry);
	System::Blip::Blink::Update(registry);
	System::Blip::Glitch::Update(context);
	System::Receiver::CommandProcessor::Update(context);
	System::Receiver::Interpret::Fire::Update(context);
	System::Receiver::CodeResponse::Update(context);
	System::Radio::Emitter::Update(context);
	System::Logic::Lever::Update(context);
	System::Logic::Breaker::Restart::Update(context, anomalyState);
	System::Logic::Breaker::Operation::Update(context);
	System::Logic::Breaker::Display::Update(context);
	System::Artillery::Aiming::Update(context);
	System::Projectile::Hit::Update(context);
	System::Anomaly::Roamer::Spawning::Update(context, anomalyState);
	System::Anomaly::Roamer::Behaviour::Update(registry, anomalyState, deltaTime);
	RoamerKillSystem::Update(registry, gameState, deltaTime);
	AnomalyAttractionSystem::Update(gameState, deltaTime);
}


void Game::DrawGame(float deltaTime)
{
#ifdef DEBUG_BUILD
	const auto view = registry.view<Component::Debug::RuntimeReadouts>();
	for (auto [entity, readouts] : view.each())
		deltaTime *= readouts.timeScale;
#endif

	RadarRenderSystem::DrawRenderTexture(
		registry, renderContext.radarRenderTexture, gameState.currentScene, resourceStore
	);

	Nc::Vector2f cameraPosition = Nc::Vector2f::Zero();
	cameraPosition.x = std::cosf(gameState.time * 0.6f) * 5.0f;
	cameraPosition.y = std::sinf((gameState.time * 2.4f) - 0.2f) * 4.0f;

	BeginTextureMode(renderContext.renderTexture);
	ClearBackground(BLANK);

	// TODO: Render light at same game size.

	const Shader& shader = resourceStore.GetShader("assets/lighting.fs");
	LightingSystem::Update(registry, renderContext.lightingContext, shader, gameState, cameraPosition, deltaTime);
	RenderingSystem::DrawScreen(registry, renderContext, gameState, cameraPosition);
	RadarRenderSystem::DrawRadar(
		registry, renderContext.radarRenderTexture, cameraPosition, gameState.currentScene
	);

	EndTextureMode();

	BeginDrawing();
	ClearBackground(Color(Palette::BACKGROUND_COLOR));

	BeginShaderMode(shader);
	DrawRenderTexture();
	EndShaderMode();

	RenderingSystem::DrawUi(registry, resourceStore, renderContext, gameState);

#ifdef DEBUG_BUILD
	DrawDebugUi();
#endif // DEBUG_BUILD

	EndDrawing();
}


void Game::Death(const SceneContext& context)
{
	Structure::RestartMenu::Open(context);
}


void Game::DrawRenderTexture() const
{
	constexpr auto DISPLAY_SIZE = Nc::Vector2f(Nc::RENDER_RESOLUTION);
	constexpr Rectangle SOURCE { 0, 0, DISPLAY_SIZE.x, -DISPLAY_SIZE.y };

	DrawTexturePro(
		renderContext.renderTexture.texture,
		SOURCE,
		renderContext.renderRectangle,
		Vector2(Nc::Vector2f::Zero()),
		0.0f,
		WHITE
	);
}


#ifdef DEBUG_BUILD
void Game::DrawDebugUi()
{
	const auto view = registry.view<Component::Debug::RuntimeReadouts>();
	for (auto [entity, readouts] : view.each())
	{

		readouts.fpsHistory.pop_back();
		readouts.fpsHistory.push_front(static_cast<int16_t>(GetFPS()));
		
		int fpsTotal = 0;
		for (const int16_t fps : readouts.fpsHistory)
			fpsTotal += fps;

		const int averageFps = fpsTotal / 32;
		std::string text = "FPS: " + std::to_string(averageFps);
		DrawText(text.c_str(), 32, 32, 32, GREEN);

		text = "MSG: " + readouts.receiverMessage;
		DrawText(text.c_str(), 32, 70, 32, GREEN);

		text = "PULSE: ";
		switch (readouts.pulse)
		{
		case MorseCode::Invalid:
			break;
		case MorseCode::Short:
			text += ".";
			break;
		case MorseCode::Long:
			text += "-";
			break;
		}
		DrawText(text.c_str(), 32, 110, 32, GREEN);

		text = "DNGER LVL: " + std::to_string(gameState.anomalyState.intensityLevel);
		DrawText(text.c_str(), 32, 148, 32, GREEN);

		text = "ATRCTION: " + std::to_string(static_cast<int32_t>(gameState.anomalyState.attractionPercentage)) + "%";
		DrawText(text.c_str(), 32, 186, 32, GREEN);

		text = "RADAR: " + std::to_string(static_cast<int32_t>(readouts.radarStabilityPercentage)) + "%";
		DrawText(text.c_str(), 32, 224, 32, GREEN);

		text = "TIME SPD: " + std::to_string(readouts.timeScale);
		DrawText(text.c_str(), 32, 272, 32, GREEN);

		text = "DAY & H: " + std::to_string(gameState.day) + " / " + std::to_string(static_cast<int32_t>(gameState.hour));
		DrawText(text.c_str(), 32, 320, 32, GREEN);

		DrawText("Press [/] to delete msg", 32, 540, 24, GREEN);
		DrawText("Press [.] to spawn roamer", 32, 580, 24, GREEN);
		DrawText("Press [G] to glitch a blip", 32, 620, 24, GREEN);
		DrawText("Press [P] to kill player", 32, 660, 24, GREEN);
		DrawText("Press [-/=] to mod intensity", 32, 700, 18, GREEN);
		DrawText("Press [9/0] to mod attraction", 32, 740, 18, GREEN);
		DrawText("Press [K/L] to mod radar stability", 32, 780, 18, GREEN);
		DrawText("Press [M/N] to mod time scale", 32, 820, 18, GREEN);
	}
}
#endif // DEBUG_BUILD
