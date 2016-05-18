
add_executable(test_stockfighter main.cpp
    test_api.cpp
    test_game.cpp
    )

target_link_libraries(test_stockfighter stockfighter)