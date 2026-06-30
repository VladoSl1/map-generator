add_library(map_gen_lib STATIC)

target_sources(map_gen_lib
    PRIVATE
        core/config.hpp
        core/utils/random.hpp
        core/utils/debug.hpp

        core/math/math.hpp
        core/math/math.cpp

        generation/pipeline/pipeline.hpp
        generation/pipeline/pipeline.cpp

        generation/pipeline/point_sampling.hpp
        generation/pipeline/point_sampling.cpp

        generation/pipeline/delaunay_triangulation.hpp
        generation/pipeline/delaunay_triangulation.cpp

        generation/pipeline/voronoi_diagram.hpp
        generation/pipeline/voronoi_diagram.cpp

        rendering/renderer.hpp
        rendering/renderer.cpp
)

# use PUBLIC so both the app and the tests can see these includes
target_include_directories(map_gen_lib
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(map_gen_lib
    PUBLIC
        raylib
        Delaunator
)

add_executable(realistic_map_generator
	main.cpp
)
target_link_libraries(realistic_map_generator
	PRIVATE
		map_gen_lib
)
