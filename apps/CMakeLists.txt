add_executable(client
	main.cpp
	../src/game_client.cpp
)

target_link_libraries(client PRIVATE
	mscore
	msui
	lan
)

