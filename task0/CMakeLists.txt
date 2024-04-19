cmake_minimum_required(VERSION 3.0)
project(SocketProject)

find_package(Threads REQUIRED)

add_executable(server server.cpp)
target_link_libraries(server PRIVATE Threads::Threads)

add_executable(client1 client1.cpp)
add_executable(client2 client2.cpp)
add_executable(client3 client3.cpp)

file(WRITE "${CMAKE_BINARY_DIR}/clients.sh"
"#!/bin/bash\n"
"./client1 > ${CMAKE_BINARY_DIR}/client1_output.txt &\n"
"./client2 > ${CMAKE_BINARY_DIR}/client2_output.txt &\n"
"./client3 > ${CMAKE_BINARY_DIR}/client3_output.txt &\n"
)

file(COPY "${CMAKE_BINARY_DIR}/clients.sh" DESTINATION "${CMAKE_BINARY_DIR}" FILE_PERMISSIONS OWNER_READ OWNER_WRITE OWNER_EXECUTE GROUP_READ GROUP_EXECUTE WORLD_READ WORLD_EXECUTE)