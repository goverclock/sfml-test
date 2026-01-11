add_executable(lobby
	lobby.cpp
	../src/game_client.cpp
)

if(WIN32)
	# to make std::println work on windows
	# see https://stackoverflow.com/questions/78452682/stdprintln-print-not-working-in-winlibs-mingw-gcc-14-1
	target_link_libraries(lobby PRIVATE "-lstdc++exp")
endif()
target_link_libraries(lobby PRIVATE
	msui
)

add_executable(minesweeper
	minesweeper.cpp
)
target_link_libraries(minesweeper PRIVATE
	raylib
)

