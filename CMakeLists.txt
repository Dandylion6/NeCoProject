cmake_minimum_required (VERSION 3.10)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

if (POLICY CMP0141)
  cmake_policy(SET CMP0141 NEW)
  set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT "$<IF:$<AND:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>,$<$<CONFIG:Debug,RelWithDebInfo>:EditAndContinue>,$<$<CONFIG:Debug,RelWithDebInfo>:ProgramDatabase>>")
endif()

set_property(DIRECTORY APPEND PROPERTY CMAKE_CONFIGURE_DEPENDS "${CMAKE_SOURCE_DIR}/include")

project ("NeCoProject")

if(NOT CMAKE_SIZEOF_VOID_P EQUAL 8)
    message(FATAL_ERROR "This project supports only 64-bit (x64) builds.")
endif()

add_subdirectory("external/raylib")
add_subdirectory("external/json")

# Grab all cpp files automatically
#[[file(GLOB_RECURSE PROJECT_SOURCES CONFIGURE_DEPENDS
    "src/*.cpp"
)

add_executable(NeCoProject
	${PROJECT_SOURCES}
)]]

add_executable(NeCoProject
		src/game/game.cpp
		src/game/utility/rendering.cpp
		src/game/system/ui/move_transition_system.cpp
		src/game/system/ui/interactive/increment_value_system.cpp
		src/game/system/ui/buttons/increment_buttons_system.cpp
		src/game/system/ui/buttons/menu_buttons_system.cpp
		src/game/system/ui/buttons/restart_buttons_system.cpp
		src/game/system/ui/buttons/settings_buttons_system.cpp
		src/game/system/shared/mechanical/breaker/breaker_display_system.cpp
		src/game/system/shared/mechanical/breaker/breaker_operation_system.cpp
		src/game/system/shared/mechanical/breaker/breaker_restart_system.cpp
		src/game/system/shared/mechanical/lever_system.cpp
		src/game/system/shared/mechanical/machine_power_system.cpp
		src/game/system/shared/input/move_region_system.cpp
		src/game/system/shared/anomaly/roamer/behaviour/phantom_behaviour_system.cpp
		src/game/system/shared/anomaly/roamer/behaviour/phaser_behaviour_system.cpp
		src/game/system/shared/anomaly/roamer/behaviour/strider_behaviour_system.cpp
		src/game/system/shared/anomaly/roamer/roamer_behaviour_system.cpp
		src/game/system/shared/anomaly/roamer/roamer_kill_system.cpp
		src/game/system/shared/anomaly/roamer/roamer_spawning_system.cpp
		src/game/system/shared/anomaly/anomaly_attraction_system.cpp
		src/game/system/scene/outside_scene/receiver/adjust_interpreting_system.cpp
		src/game/system/scene/outside_scene/receiver/interpret_aiming_system.cpp
		src/game/system/scene/outside_scene/receiver/interpret_coordinate_system.cpp
		src/game/system/scene/outside_scene/receiver/interpret_fire_system.cpp
		src/game/system/scene/outside_scene/receiver/interpret_recalibration_system.cpp
		src/game/system/scene/outside_scene/receiver/receiver_code_response_system.cpp
		src/game/system/scene/outside_scene/receiver/receiver_command_processor_system.cpp
		src/game/system/scene/outside_scene/artillery/projectile_hit_system.cpp
		src/game/system/scene/outside_scene/artillery/artillery_aiming_system.cpp
		src/game/system/scene/comms_scene/radar/radar_artillery_system.cpp
		src/game/system/scene/comms_scene/radar/radar_buttons_system.cpp
		src/game/system/scene/comms_scene/radar/radar_render_system.cpp
		src/game/system/scene/comms_scene/radar/radar_stability_system.cpp
        src/game/system/scene/comms_desk_scene/morse_code/morse_recording_system.cpp
        src/game/system/scene/comms_desk_scene/morse_code/morse_tone_system.cpp
        src/game/system/scene/comms_desk_scene/morse_code/morse_monitor_display_system.cpp
		src/game/system/scene/comms_scene/radio/radio_emitter_system.cpp
		src/game/system/scene/comms_scene/radar/blip/blip_blink_system.cpp
		src/game/system/scene/comms_scene/radar/blip/blip_death_system.cpp
		src/game/system/scene/comms_scene/radar/blip/blip_glitch_system.cpp
		src/game/system/scene/comms_scene/radar/blip/glitch/blip_contact_failure_system.cpp
		src/game/system/scene/comms_scene/radar/blip/glitch/blip_distortion_system.cpp
		src/game/system/scene/comms_scene/radar/blip/glitch/blip_signal_noise_system.cpp
		src/game/system/core/tween_system.cpp
		src/game/system/core/rendering/rectangle_render_system.cpp
		src/game/system/core/rendering/rendering_system.cpp
		src/game/system/core/rendering/sprite_render_system.cpp
		src/game/system/core/rendering/text_render_system.cpp
		src/game/system/core/rendering/lighting/lighting_system.cpp
		src/game/system/core/interactive/click_action_system.cpp
		src/game/system/core/interactive/drag_action_system.cpp
		src/game/system/core/interactive/input_action_system.cpp
		src/game/system/core/audio/audio_emitter_system.cpp
		src/game/system/core/audio/main_ambience_system.cpp
		src/game/save/save_game.cpp
		src/game/save/save_settings.cpp
		src/game/construction/ui/main_menu/entity/menu_background_entity.cpp
		src/game/construction/ui/main_menu/object/menu_button_objects.cpp
		src/game/construction/ui/main_menu/main_menu.cpp
		src/game/construction/ui/restart_menu/object/restart_background_object.cpp
		src/game/construction/ui/restart_menu/object/restart_button_objects.cpp
		src/game/construction/ui/restart_menu/restart_menu.cpp
		src/game/construction/ui/settings_menu/entity/settings_header_entities.cpp
		src/game/construction/ui/settings_menu/object/increment_setting_object.cpp
		src/game/construction/ui/settings_menu/object/settings_background_object.cpp
		src/game/construction/ui/settings_menu/object/settings_button_objects.cpp
		src/game/construction/ui/settings_menu/settings_menu.cpp
		src/game/construction/ui/shared/entity/move_transition_entity.cpp
		src/game/construction/ui/shared/object/label_button_object.cpp
		src/game/construction/shared/entity/environment/aimbient_sound_entity.cpp
		src/game/construction/shared/entity/environment/light_source_entity.cpp
		src/game/construction/shared/entity/interactive/lever_entity.cpp
		src/game/construction/shared/entity/scene/move_region_entity.cpp
		src/game/construction/shared/entity/scene/scene_background_entity.cpp
		src/game/construction/shared/object/interactive/circuit_breaker_object.cpp
		src/game/construction/scene/comms_desk_scene/entity/note_entity.cpp
		src/game/construction/scene/comms_desk_scene/comms_desk_scene.cpp
		src/game/construction/scene/comms_scene/entity/blip_entity.cpp
		src/game/construction/scene/comms_desk_scene/entity/morse_transceiver_entity.cpp
		src/game/construction/scene/comms_scene/entity/radio_entity.cpp
		src/game/construction/scene/comms_desk_scene/object/morse_monitor_object.cpp
		src/game/construction/scene/comms_scene/object/radar_object.cpp
		src/game/construction/scene/comms_scene/comms_scene.cpp
		src/game/construction/scene/doorway_scene/object/radar_breaker_object.cpp
		src/game/construction/scene/doorway_scene/doorway_scene.cpp
		src/game/construction/scene/outside_scene/entity/artillery_entity.cpp
		src/game/construction/scene/outside_scene/entity/projectile_entity.cpp
		src/game/construction/scene/outside_scene/outside_scene.cpp
		src/core/data/bounds.cpp
		src/core/runtime/resource_store.cpp
		include/game/system/core/rendering/lighting/light_flickering_system.hpp
		src/game/system/core/rendering/lighting/light_flickering_system.cpp
		include/game/system/scene/comms_desk_scene/morse_code/morse_input_system.hpp
		src/game/system/scene/comms_desk_scene/morse_code/morse_input_system.cpp
        include/game/system/ui/interactive/settings_input_system.hpp
        src/game/system/ui/interactive/settings_input_system.cpp
        src/game/system/scene/comms_scene/radar/radar_screen_glitch_system.cpp
        include/game/construction/shared/entity/scene/scene_ambience_entity.hpp
        src/game/construction/shared/entity/scene/scene_ambience_entity.cpp
        include/game/component/core/audio/audio_modifier_component.hpp)

target_sources(NeCoProject PRIVATE
		src/main.cpp
)


target_compile_definitions(
	NeCoProject 
	PRIVATE
    	BUILD_DIR_PATH="${CMAKE_BINARY_DIR}"
)

target_compile_definitions(
	NeCoProject
	PRIVATE 
		$<$<CONFIG:Debug>:DEBUG_BUILD>
		$<$<CONFIG:Release>:RELEASE_BUILD>
)

target_include_directories(
	NeCoProject
	PRIVATE
		$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
		$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/external/base64/include>
		$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/external/entt/src>
		$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/external/raylib/src>
		$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/external/json/src>
)

target_link_libraries(
	NeCoProject 
	PRIVATE
		raylib
		nlohmann_json
)

# Copy assets after build
add_custom_command(TARGET NeCoProject POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_SOURCE_DIR}/assets
        $<TARGET_FILE_DIR:NeCoProject>/assets
)