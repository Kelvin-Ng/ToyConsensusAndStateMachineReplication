project(ToyConsensusAndStateMachineReplication)
cmake_minimum_required(VERSION 3.2)

set(PROTO_INCLUDE_DIR ${CMAKE_CURRENT_BINARY_DIR}/proto-gen)
set(PROTO_DST_DIR ${CMAKE_CURRENT_BINARY_DIR}/proto-gen/tcsmr)
file(MAKE_DIRECTORY ${PROTO_DST_DIR})

add_subdirectory(src)

