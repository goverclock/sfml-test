add_library(minesweeper minesweeper.cpp)

target_link_libraries(minesweeper PRIVATE raylib)

target_compile_features(minesweeper PUBLIC cxx_std_17)
