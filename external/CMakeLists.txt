add_library(Delaunator INTERFACE)

target_sources(Delaunator
	PUBLIC
		FILE_SET HEADERS
		FILES
			delaunator.hpp
)
