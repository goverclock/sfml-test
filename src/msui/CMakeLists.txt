add_library(libmsui local_ui.cpp)

include(FetchContent)
FetchContent_Declare(SFML
    GIT_REPOSITORY https://github.com/SFML/SFML.git
    GIT_TAG 3.0.2
    GIT_SHALLOW ON
    EXCLUDE_FROM_ALL
    SYSTEM)
FetchContent_MakeAvailable(SFML)

target_link_libraries(libmsui PUBLIC
	SFML::Graphics
	libmscore
)

target_compile_features(libmsui PUBLIC cxx_std_17)

