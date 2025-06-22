cmake_minimum_required (VERSION 3.10)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

if (POLICY CMP0141)
  cmake_policy(SET CMP0141 NEW)
  set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT "$<IF:$<AND:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>,$<$<CONFIG:Debug,RelWithDebInfo>:EditAndContinue>,$<$<CONFIG:Debug,RelWithDebInfo>:ProgramDatabase>>")
endif()

project ("NeCoProject")

if(NOT CMAKE_SIZEOF_VOID_P EQUAL 8)
    message(FATAL_ERROR "This project supports only 64-bit (x64) builds.")
endif()

add_subdirectory("external/raylib")

# Grab all cpp files automatically
file(GLOB_RECURSE PROJECT_SOURCES CONFIGURE_DEPENDS
    "src/*.cpp"
)

add_executable(NeCoProject ${PROJECT_SOURCES} "include/systems/object/receiver/artillery_control_system.h" "src/systems/object/receiver/artillery_control_system.cpp" "include/systems/object/receiver/receiver_code_response_system.h" "src/systems/object/receiver/receiver_code_response_system.cpp" "include/systems/object/radio_sound_system.h" "src/systems/object/radio_sound_system.cpp")

target_include_directories(
	NeCoProject
	PRIVATE
		"include"
		"external/entt/src"
		"external/raylib/src"
)

target_link_libraries(
	NeCoProject 
	PRIVATE
		raylib
)

# Copy assets after build
add_custom_command(TARGET NeCoProject POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_SOURCE_DIR}/assets
        $<TARGET_FILE_DIR:NeCoProject>/assets
)
