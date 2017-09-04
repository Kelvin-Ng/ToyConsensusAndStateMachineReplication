include_directories("${CMAKE_SOURCE_DIR}/include")

add_subdirectory(utils)
add_subdirectory(net)

set(base-objs $<TARGET_OBJECTS:base-utils> $<TARGET_OBJECTS:base-net> PARENT_SCOPE)

