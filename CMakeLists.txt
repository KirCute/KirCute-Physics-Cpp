cmake_minimum_required(VERSION 3.8)

project("KirCute-Physics-Cpp")

include_directories(
        include
        include/KirCute-Physics-Cpp
        include/Easyx
)

add_library(kircute_physics
        include/KirCute-Physics-Cpp/frame.h
        include/KirCute-Physics-Cpp/world.h
        include/KirCute-Physics-Cpp/rbody.h
        include/KirCute-Physics-Cpp/store.h
        include/KirCute-Physics-Cpp/utils.h
        include/KirCute-Physics-Cpp/render.h

        src/frame.cpp
        src/rbody.cpp
        src/world.cpp
        )

add_library(easyx_renderer
        include/KirCute-Physics-Cpp/easyx_renderer.h
        src/easyx_renderer.cpp
        )

add_library(quadtree_store
        include/KirCute-Physics-Cpp/quadtree_store.h
        src/quadtree_store.cpp
        )

add_library(vector_store
        include/KirCute-Physics-Cpp/vector_store.h
        src/vector_store.cpp
        )

link_directories(lib/x64)
link_libraries(kircute_physics easyx_renderer vector_store quadtree_store)

add_executable(QuadtreeDemo demo/QuadtreeDemo.cpp)
add_executable(WaveDemo demo/WaveDemo.cpp)