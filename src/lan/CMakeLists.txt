add_library(lan lan.cpp)

target_link_libraries(lan PUBLIC SFML::Network)

target_compile_features(lan PUBLIC cxx_std_17)
