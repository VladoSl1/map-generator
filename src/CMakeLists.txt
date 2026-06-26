
add_executable(realistic_map_generator)

target_sources(realistic_map_generator
	PRIVATE
		main.cpp
		core/config.hpp
		core/utils/random.hpp
		generation/pipeline/pipeline.hpp
		generation/pipeline/pipeline.cpp
		generation/pipeline/point_sampling.hpp
		generation/pipeline/point_sampling.cpp
		rendering/renderer.hpp
		rendering/renderer.cpp

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
