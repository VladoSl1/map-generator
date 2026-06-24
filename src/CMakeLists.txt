
add_executable(realistic_map_generator)

target_sources(realistic_map_generator
	PRIVATE
		main.cpp
)

target_link_libraries(realistic_map_generator
	PRIVATE
		raylib
)
