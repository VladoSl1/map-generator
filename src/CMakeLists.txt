
add_executable(realistic_map_generator)

target_sources(realistic_map_generator
	PRIVATE
		main.cpp
		core/config.hpp
		core/utils/random.hpp
		generation/pipeline.hpp
		rendering/renderer.hpp

)

target_include_directories(realistic_map_generator
	PRIVATE
		${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(realistic_map_generator
	PRIVATE
		raylib
		Delaunator
)
