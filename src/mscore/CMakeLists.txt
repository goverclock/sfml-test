add_library(mscore local_status.cpp)

target_compile_features(mscore PUBLIC cxx_std_17)

target_link_libraries(mscore PUBLIC lan)
