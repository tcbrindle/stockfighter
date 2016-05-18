

add_library(stockfighter
    api.cpp
    game.cpp
    rest.cpp
    )

target_include_directories(stockfighter PRIVATE ${FMT_INCLUDE_DIRS})
target_include_directories(stockfighter PRIVATE ${OPENSSL_INCLUDE_DIR})
target_include_directories(stockfighter PRIVATE ${CPPNETLIB_INCLUDE_DIRS})

target_compile_definitions(stockfighter PRIVATE "-DBOOST_NETWORK_ENABLE_HTTPS")
target_compile_definitions(stockfighter PRIVATE "-DFMT_HEADER_ONLY")

target_link_libraries(stockfighter
    ${FMT_LIBRARIES}
    ${CPPNETLIB_LIBRARIES}
    )
