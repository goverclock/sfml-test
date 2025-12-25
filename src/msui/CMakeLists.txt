add_library(msui local_ui.cpp)

target_link_libraries(msui PUBLIC
	SFML::Graphics
	mscore
	widget
)

target_compile_features(msui PUBLIC cxx_std_17)

