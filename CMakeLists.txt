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
file(GLOB_RECURSE PROJECT_SOURCES CONFIGURE_DEPENDS
    "src/*.cpp"
)

add_executable(NeCoProject 
	${PROJECT_SOURCES} 
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