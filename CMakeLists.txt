cmake_minimum_required (VERSION 3.10)
project(earth_observation_satellites)

file(GLOB UTILS utils/*.cpp)
file(GLOB SATELLITE_MOTION satellite_motion/*/*.cpp)
file(GLOB CREATORS creators/*.cpp)
file(GLOB OBSERVATION creators/*.cpp)

add_executable(run 
    main.cpp
    ${UTILS}
    ${SATELLITE_MOTION}
    ${CREATORS}
    ${OBSERVATION}
    )
