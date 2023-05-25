cmake_minimum_required(VERSION 3.22)

project(bulk_server_p LANGUAGES CXX)

add_executable(bulk_server_e
    main.cpp
    server.h
)

if(MINGW)
  set (BOOST_ROOT "C:/Program_/Boost/boost_1.81.0_new_MGW")
  set (Boost_COMPILER mgw8)
endif()

find_package(Threads REQUIRED)
find_package(Boost REQUIRED system)

if(WIN32)
  target_link_libraries(bulk_server_e PUBLIC ws2_32 wsock32)
endif()

target_include_directories(bulk_server_e PRIVATE "../lib/libasync")
target_link_libraries(bulk_server_e PRIVATE libasync_l)
target_link_libraries(bulk_server_e PUBLIC ${Boost_LIBRARIES} Threads::Threads)






