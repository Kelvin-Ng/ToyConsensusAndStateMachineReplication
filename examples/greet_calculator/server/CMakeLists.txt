include_directories(${TCSMR_INCLUDE_DIR})
link_directories(${TCSMR_LIB_DIR})

add_executable(server server.cpp $<TARGET_OBJECTS:protos>)

target_link_libraries(server
    tcsmr
    gRPC::grpc++
    protobuf::libprotobuf
    glog
)

