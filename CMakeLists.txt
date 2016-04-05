cmake_minimum_required(VERSION 3.3)
project(3)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILES
    GL/freeglut.h
    GL/freeglut_ext.h
    GL/freeglut_std.h
    vecmath/include/Matrix2f.h
    vecmath/include/Matrix3f.h
    vecmath/include/Matrix4f.h
    vecmath/include/Quat4f.h
    vecmath/include/vecmath.h
    vecmath/include/Vector2f.h
    vecmath/include/Vector3f.h
    vecmath/include/Vector4f.h
    vecmath/src/Matrix2f.cpp
    vecmath/src/Matrix3f.cpp
    vecmath/src/Matrix4f.cpp
    vecmath/src/Quat4f.cpp
    vecmath/src/Vector2f.cpp
    vecmath/src/Vector3f.cpp
    vecmath/src/Vector4f.cpp
    camera.cpp
    camera.h
    ClothSystem.cpp
    ClothSystem.h
    Forcefield.cpp
    Forcefield.h
    main.cpp
    particleSystem.cpp
    particleSystem.h
    pendulumSystem.cpp
    pendulumSystem.h
    simpleSystem.cpp
    simpleSystem.h
    spring.cpp
    Spring.h
    TimeStepper.cpp
    TimeStepper.hpp)

add_executable(3 ${SOURCE_FILES})