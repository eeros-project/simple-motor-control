cmake_minimum_required(VERSION 3.14)

project(simple-motor-control LANGUAGES CXX)

find_package(EEROS 1.4 REQUIRED)

if(USE_FLINK)
  find_package(flink 1.1 REQUIRED)
  find_package(flink-eeros 1.0 REQUIRED)
endif(USE_FLINK)

if(USE_BBBLUE)
  find_package(bbblue-eeros 1.3 REQUIRED)
endif(USE_BBBLUE)

if(USE_COMEDI)
  find_package(comedi-eeros 0.2 REQUIRED)
endif(USE_COMEDI)

add_custom_command(OUTPUT copyHalConfig POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy "${simpleMotorControl_SOURCE_DIR}/*.json" "${simpleMotorControl_BINARY_DIR}")

add_executable(${PROJECT_NAME} SimpleMotorControl.cpp ControlSystem.cpp SMCSafetyProperties.cpp copyHalConfig)
target_compile_features(${PROJECT_NAME} PRIVATE cxx_std_14)
target_link_libraries(${PROJECT_NAME} PRIVATE EEROS::eeros)

install(TARGETS ${PROJECT_NAME} DESTINATION bin)

