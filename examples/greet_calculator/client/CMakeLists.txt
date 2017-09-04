add_executable(client client.cpp $<TARGET_OBJECTS:protos>)

target_link_libraries(client
    gRPC::grpc++
    protobuf::libprotobuf
    glog
)

