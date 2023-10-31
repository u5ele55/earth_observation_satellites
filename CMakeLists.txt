cmake_minimum_required (VERSION 3.10)
project(earth_observation_satellites)

file(GLOB UTILS utils/*.cpp)
file(GLOB SATELLITE_MOTION_MODELING satellite_motion/modeling/*.cpp)
file(GLOB SATELLITE_MOTION_SYSTEM satellite_motion/system/*.cpp)

add_executable(run 
    main.cpp
    ${UTILS}
    ${SATELLITE_MOTION_MODELING}
    ${SATELLITE_MOTION_SYSTEM}
    )
