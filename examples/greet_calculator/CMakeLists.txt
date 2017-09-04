project(ToyConsensusAndStateMachineReplication-GreetCalculator)
cmake_minimum_required(VERSION 3.2)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake")
find_package(Protobuf REQUIRED)
find_package(GRPC REQUIRED)

set(PROTO_INCLUDE_DIR ${CMAKE_CURRENT_BINARY_DIR}/protos-gen)
set(PROTO_DST_DIR ${CMAKE_CURRENT_BINARY_DIR}/protos-gen)
file(MAKE_DIRECTORY ${PROTO_DST_DIR})

include_directories(${PROTO_INCLUDE_DIR})

add_subdirectory(protos)
add_subdirectory(server)
add_subdirectory(client)

