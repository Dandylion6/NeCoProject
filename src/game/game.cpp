#include "game/game.hpp"

#include <cmath>

#include "raylib.h"
#include "core/data/vector2.hpp"
#include "core/math/nc_math.hpp"
#include "core/math/random.hpp"
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
#include "game/save/save_game.hpp"
#include "game/save/save_settings.hpp"
#include "game/state/game_state.hpp"
#include "game/system/core/tween_system.hpp"
#include "game/system/core/audio/audio_emitter_system.hpp"
#include "game/system/core/audio/main_ambience_system.hpp"
#include "game/system/core/interactive/click_action_system.hpp"
#include "game/system/core/interactive/drag_action_system.hpp"
#include "game/system/core/interactive/input_action_system.hpp"
#include "game/system/core/rendering/rendering_system.hpp"
#include "game/system/core/rendering/lighting/lighting_system.hpp"
#include "game/system/core/rendering/lighting/light_flickering_system.hpp"
#include "game/system/scene/comms_desk_scene/morse_code/morse_input_system.hpp"
#include "game/system/scene/comms_desk_scene/morse_code/morse_monitor_display_system.hpp"
#include "game/system/scene/comms_desk_scene/morse_code/morse_recording_system.hpp"
#include "game/system/scene/comms_desk_scene/morse_code/morse_tone_system.hpp"
#include "game/system/scene/comms_scene/radar/radar_artillery_system.hpp"
#include "game/system/scene/comms_scene/radar/radar_buttons_system.hpp"
#include "game/system/scene/comms_scene/radar/radar_render_system.hpp"
#include "game/system/scene/comms_scene/radar/radar_screen_glitch_system.hpp"
#include "game/system/scene/comms_scene/radar/radar_stability_system.hpp"
#include "game/system/scene/comms_scene/radar/blip/blip_blink_system.hpp"
#include "game/system/scene/comms_scene/radar/blip/blip_death_system.hpp"
#include "game/system/scene/comms_scene/radar/blip/blip_glitch_system.hpp"
#include "game/system/scene/comms_scene/radar/blip/glitch/blip_contact_failure_system.hpp"
#include "game/system/scene/comms_scene/radar/blip/glitch/blip_signal_noise_system.hpp"
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
#include "game/system/shared/anomaly/roamer/behaviour/phantom_behaviour_system.hpp"
#include "game/system/shared/anomaly/roamer/behaviour/phaser_behaviour_system.hpp"
#include "game/system/shared/anomaly/roamer/behaviour/strider_behaviour_system.hpp"
#include "game/system/shared/input/move_region_system.hpp"
#include "game/system/shared/mechanical/lever_system.hpp"
#include "game/system/shared/mechanical/machine_power_system.hpp"
#include "game/system/shared/mechanical/breaker/breaker_display_system.hpp"
#include "game/system/shared/mechanical/breaker/breaker_operation_system.hpp"
#include "game/system/shared/mechanical/breaker/breaker_restart_system.hpp"
#include "game/system/ui/move_transition_system.hpp"
#include "game/system/ui/buttons/increment_buttons_system.hpp"
#include "game/system/ui/buttons/menu_buttons_system.hpp"
#include "game/system/ui/buttons/restart_buttons_system.hpp"
#include "game/system/ui/buttons/settings_buttons_system.hpp"
#include "game/system/ui/interactive/increment_value_system.hpp"
#include "game/system/ui/interactive/settings_input_system.hpp"
#include "game/tag/core/life_cycle/dont_destroy_on_load_tag.hpp"
#include "game/utility/color_palette.hpp"

#ifdef DEBUG_BUILD
#include <cstring>
#include <string>

#include "game/component/shared/debug/dev_settings_component.hpp"
#include "game/component/shared/debug/runtime_readouts_component.hpp"
#include "game/state/scene.hpp"
#include "game/utility/morse_code.hpp"
#endif


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

	System::Render::Lighting::Initialize(renderContext.lightingContext, resourceStore);
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
    gameState.isPaused = false;
}


void Game::BuildMenuUI()
{
	const auto context = SceneContext(registry, resourceStore, gameState);
	const auto windowSize = Nc::Vector2f(renderContext.windowSize);

	//Entity::AmbientSound::Create(registry);
	Entity::MoveTransition::Create(context, renderContext);

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
	Structure::MainMenu::Open(context);
#endif
}


void Game::BuildRuntimeScenes()
{
	const BuildContext buildContext = BuildContext(registry, resourceStore, renderContext, gameState);

	Structure::CommsScene::Build(buildContext);
	Structure::DeskScene::Build(buildContext);
	Structure::DoorwayScene::Build(buildContext);
	Structure::OutsideScene::Build(buildContext);
}


void Game::SetupWindow()
{
#ifdef DEBUG_BUILD
	const entt::entity entity = entt::get_single<Component::Debug::DevSettings>(registry);
	const auto& devSettings = registry.get<Component::Debug::DevSettings>(entity);

	if (!devSettings.isMaximizedWindowed)
	{
		SetConfigFlags(FLAG_VSYNC_HINT | FLAG_FULLSCREEN_MODE);
	}

#else
	SetConfigFlags(FLAG_VSYNC_HINT | FLAG_FULLSCREEN_MODE);
#endif

	const auto monitorSize = Nc::Vector2i(GetMonitorWidth(0), GetMonitorHeight(0));
	InitWindow(monitorSize.x, monitorSize.y, "Negative Contact");

#ifdef DEBUG_BUILD
	SetExitKey(KEY_BACKSPACE);

	if (devSettings.isMaximizedWindowed)
	{
		SetWindowState(FLAG_WINDOW_RESIZABLE);
		MaximizeWindow();
	}

#else
	SetExitKey(KEY_NULL);
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
	if (gameEvents.shouldExit) return false;
	return true;
}


void Game::Update(float deltaTime)
{
	constexpr float NIGHT_END_HOUR = GameState::NIGHT_RANGE.y + 1.0f;

#ifdef DEBUG_BUILD
	if (IsKeyPressed(KEY_PERIOD))
	{
		auto& randomService = registry.ctx().get<Nc::Random>();
		const Nc::Vector2f spawnPoint = System::Anomaly::Roamer::Spawning::GenerateRandomSpawnPoint(randomService);
		System::Anomaly::Roamer::Spawning::SpawnRoamer(
			{registry, resourceStore, gameState, gameEvents, deltaTime},
			spawnPoint,
			anomalyState
		);
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

	if (IsKeyPressed(KEY_P)) Death({registry, resourceStore, gameState});
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


void Game::UpdateSystems(float deltaTime)
{
#ifdef DEBUG_BUILD
	const entt::entity debugEntity = entt::get_single<Component::Debug::RuntimeReadouts>(registry);
	const auto& readouts = registry.get<Component::Debug::RuntimeReadouts>(debugEntity);
	deltaTime *= readouts.timeScale;
#endif

	const auto context = SystemContext(registry, resourceStore, gameState, gameEvents, deltaTime);

	System::Tween::Update(context);
	System::Action::Input::Update(context);
	System::Action::Click::Update(context, renderContext);
	System::Action::Drag::Update(context, renderContext);
	System::UI::IncrementValue::Update(registry);
	System::UI::MoveTransition::Update(context);
	//System::Audio::MainAmbience::Update(context);
	System::Audio::Emitter::Update(context);

	System::Restart::Buttons::Update(context);
	System::Menu::Buttons::Update(context);
	System::Settings::Buttons::Update(context, settings, pendingSettings);
    System::Settings::Input::Update(context);
	System::Input::MoveRegion::Update(context);
	System::UI::IncrementButtons::Update(context);

	MouseCursor cursor = MOUSE_CURSOR_DEFAULT;
	switch (gameState.cursor)
	{
	case Nc::Cursor::Clickable:
		cursor = MOUSE_CURSOR_POINTING_HAND;
		break;
	case Nc::Cursor::Grab:
		cursor = MOUSE_CURSOR_POINTING_HAND;
		break;
	default: break;
	}
	SetMouseCursor(cursor);

	if (gameState.currentScene == NullScene) return;
	if (gameState.isPaused) return;

    System::Morse::Input::Update(context, anomalyState);
	System::Morse::Recording::Update(context, settings.morseSettings);
	System::Morse::MonitorDisplay::Update(context, settings.morseSettings);
	System::Morse::Tone::Update(context);

	System::Receiver::Interpret::Recalibration::Update(context);
	System::Machine::PowerUsage::Update(context, anomalyState);

	System::Radar::Buttons::Update(context);
	System::Radar::Stability::Update(context, anomalyState);
    System::Radar::ScreenGlitch::Update(context);
	System::Radar::Artillery::Update(context);

	System::Blip::Death::Update(registry);
	System::Blip::Blink::Update(registry);
	System::Blip::Glitch::Update(context);
    System::Blip::Jumble::Update(context);
    System::Blip::TextError::Update(context);
    System::Blip::ContactFailure::Update(context);

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
	System::Anomaly::Roamer::Strider::Update(context);
	System::Anomaly::Roamer::Phaser::Update(context);
	System::Anomaly::Roamer::Phantom::Update(context);
	System::Anomaly::Roamer::Behaviour::Update(context, anomalyState);
	System::Anomaly::Roamer::Kill::Update(context);

	System::Anomaly::Attraction::Update(context, anomalyState);
	System::Render::LightFlickering::Update(context);
}


void Game::DrawGame(float deltaTime)
{
	constexpr auto CAMERA_SWAY_STRENGTH = Nc::Vector2f(5.0f, 3.0f);
	constexpr auto CAMERA_SWAY_SPEED = Nc::Vector2f(0.08f, 0.5f) * Nc::Math::TWO_PI;
	constexpr auto HEIGHT_SWAY_PHASE = 0.2f;

#ifdef DEBUG_BUILD
	const entt::entity debugEntity = entt::get_single<Component::Debug::RuntimeReadouts>(registry);
	const auto& readouts = registry.get<Component::Debug::RuntimeReadouts>(debugEntity);
	deltaTime *= readouts.timeScale;
#endif

	const auto context = SystemContext(registry, resourceStore, gameState, gameEvents, deltaTime);
	System::Render::Radar::DrawRenderTexture(context, renderContext);

	auto cameraPosition = Nc::Vector2f::Zero();
	const Nc::Vector2f swayTime = CAMERA_SWAY_SPEED * gameState.time;

	cameraPosition.x = std::cos(swayTime.x) * CAMERA_SWAY_STRENGTH.x;
	cameraPosition.y = std::sin(swayTime.y - HEIGHT_SWAY_PHASE) * CAMERA_SWAY_STRENGTH.y;
	renderContext.cameraPosition = cameraPosition;

	BeginTextureMode(renderContext.renderTexture);
	ClearBackground(BLANK);

	System::Render::Radar::DrawRadar(context, renderContext);

	const Shader& shader = System::Render::Lighting::Update(context, renderContext);
	BeginShaderMode(shader);

    RenderingSystem::DrawScreen(shader, registry, gameState, cameraPosition);

	EndShaderMode();
	EndTextureMode();

	BeginDrawing();
	ClearBackground(Color(Palette::BACKGROUND_COLOR));

	DrawRenderTexture();

	RenderingSystem::DrawUi(registry, resourceStore, renderContext);

#ifdef DEBUG_BUILD
	DrawDebugUi();
#endif

	EndDrawing();
}


void Game::HandleEvents()
{
	if (gameEvents.shouldLoad || gameEvents.shouldRestart)
		Load();

	const bool shouldExit = gameEvents.shouldExit;
	gameEvents = { }; // Resets the event bus.
	gameEvents.shouldExit = shouldExit;
}


void Game::Death(const SceneContext& context)
{
    context.game.isPaused = true;
	Structure::RestartMenu::Open(context);
}


void Game::DrawRenderTexture() const
{
	constexpr auto DISPLAY_SIZE = Nc::Vector2f(Nc::RENDER_RESOLUTION);
	constexpr Rectangle SOURCE{0, 0, DISPLAY_SIZE.x, -DISPLAY_SIZE.y};

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
	const entt::entity debugEntity = entt::get_single<Component::Debug::RuntimeReadouts>(registry);
	auto& [fpsHistory, receiverMessage, radarStabilityPercentage, timeScale] = registry.get<
		Component::Debug::RuntimeReadouts>(debugEntity);

	fpsHistory.pop_back();
	fpsHistory.push_front(static_cast<int16_t>(GetFPS()));

	int fpsTotal = 0;
	for (const int16_t fps : fpsHistory)
		fpsTotal += fps;

	const int averageFps = fpsTotal / 32;
	std::string text = "FPS: " + std::to_string(averageFps);
	DrawText(text.c_str(), 32, 32, 32, GREEN);

	text = "MSG: " + receiverMessage;
	DrawText(text.c_str(), 32, 70, 32, GREEN);

	text = "DNGER LVL: " + std::to_string(anomalyState.intensityLevel);
	DrawText(text.c_str(), 32, 148, 32, GREEN);

	text = "ATRCTION: " + std::to_string(static_cast<int32_t>(anomalyState.attractionPercentage)) + "%";
	DrawText(text.c_str(), 32, 186, 32, GREEN);

	text = "RADAR: " + std::to_string(static_cast<int32_t>(radarStabilityPercentage)) + "%";
	DrawText(text.c_str(), 32, 224, 32, GREEN);

	text = "TIME SPD: " + std::to_string(timeScale);
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
#endif // DEBUG_BUILD
