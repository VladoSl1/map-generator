add_library(map_gen_lib STATIC)

target_sources(map_gen_lib
    PRIVATE
        core/config.hpp

        core/utils/random.hpp

        core/utils/debug.hpp

        core/math/geometry.hpp
        core/math/geometry.cpp

        core/math/point2d.hpp

        core/math/sorting.hpp

        core/math/topology.hpp
        core/math/topology.cpp

        generation/pipeline/pipeline.hpp
        generation/pipeline/pipeline.cpp

        generation/pipeline/point_sampling.hpp
        generation/pipeline/point_sampling.cpp

        generation/pipeline/delaunay_triangulation.hpp
        generation/pipeline/delaunay_triangulation.cpp

        generation/pipeline/voronoi_diagram.hpp
        generation/pipeline/voronoi_diagram.cpp

        generation/chunkmanager.hpp
        generation/chunkmanager.cpp

        rendering/renderer.hpp
        rendering/renderer.cpp

        rendering/camera_control.hpp
        rendering/camera_control.cpp

        rendering/chunkstreamer.hpp
        rendering/chunkstreamer.cpp
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
        FastNoise2
)

add_executable(realistic_map_generator
	main.cpp
)
target_link_libraries(realistic_map_generator
	PRIVATE
		map_gen_lib
)
