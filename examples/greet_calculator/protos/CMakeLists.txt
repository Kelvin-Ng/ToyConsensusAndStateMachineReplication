set(PROTOS
    greet_service.proto
    calculator_service.proto
)

protobuf_generate_cpp(PROTO_SRCS PROTO_HDRS ${PROTO_DST_DIR} ${PROTOS})
grpc_generate_cpp(GRPC_SRCS GRPC_HDRS ${PROTO_DST_DIR} ${PROTOS})

add_library(protos OBJECT ${PROTO_SRCS} ${GRPC_SRCS})

