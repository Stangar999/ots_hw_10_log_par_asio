cmake_minimum_required(VERSION 3.5)

project(client)

if(MINGW)
  ## путь к коревому каталогу буста (например: с:/boost)
  set (BOOST_ROOT "C:/Program_/Boost/boost_1.81.0_new_MGW")
  set (Boost_COMPILER mgw8)
endif()

find_package(Boost REQUIRED system)
find_package(Threads)

include_directories(${Boost_INCLUDE_DIR})

add_executable(client client.cpp)

set_target_properties(client PROPERTIES
        CXX_STANDARD 20
        CXX_STANDARD_REQUIRED ON
        )

if(WIN32)
  target_link_libraries(client
    ws2_32
    wsock32
  )
endif()

target_link_libraries(client ${Boost_LIBRARIES} ${CMAKE_THREAD_LIBS_INIT})

