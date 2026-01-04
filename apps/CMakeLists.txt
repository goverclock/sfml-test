add_executable(lobby
	lobby.cpp
	../src/game_client.cpp
)
target_link_libraries(lobby PRIVATE
	msui
)

add_executable(minesweeper
	minesweeper.cpp
)
target_link_libraries(minesweeper PRIVATE
	raylib
)

