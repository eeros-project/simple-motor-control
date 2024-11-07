cmake_minimum_required(VERSION 3.10)

project(simpleMotorControl)

set(CMAKE_CXX_STANDARD 14)


set(RECOMMENDED_EEROS_VERSION 1.4)
set(RECOMMENDED_SIM_EEROS_VERSION 1.0)
set(RECOMMENDED_FLINK_EEROS_VERSION 1.0)
set(RECOMMENDED_BBBLUE_EEROS_VERSION 1.3)
set(RECOMMENDED_COMEDI_EEROS_VERSION 0.2)
set(RECOMMENDED_ROS_EEROS_VERSION 1.0)


# Load CMake modules library at specified version
set(LOAD_CMAKE_MODULES_LIB TRUE CACHE BOOL "TRUE if the cmake-modules library should be loaded.")

if(LOAD_CMAKE_MODULES_LIB)
  include(cmake/CMakeModulesLibLoader.cmake)
  load_cmake_modules_lib(https://github.com/eeros-project/cmake-modules.git)
endif()

include(CMakeModulesLib)

if(CMAKE_BUILD_TYPE MATCHES "Debug|DEBUG")
  logger_on()
endif()

if(LOAD_CMAKE_MODULES_LIB)
  checkout_cmake_modules_lib_version(8fcde84)
endif()


include(VersioningHelpers)
set(REQUIRED_DEGREE_OF_VERSION_COMPATIBILITY 2 CACHE STRING "The required degree of version compatibilty.")


# Find EEROS
setVersionToFind("EEROS" "REQUIRED_EEROS_VERSION" ${RECOMMENDED_EEROS_VERSION} ${REQUIRED_EEROS_VERSION})
find_package(EEROS ${VERSION_TO_FIND})
checkIfModuleFound("EEROS" "REQUIRED_EEROS_VERSION")
checkVersionCompatibility(${EEROS_VERSION} ${VERSION_TO_FIND} ${REQUIRED_DEGREE_OF_VERSION_COMPATIBILITY})


# Find Simulator
if(USE_SIM)
  setVersionToFind("sim-eeros" "REQUIRED_SIM_EEROS_VERSION" ${RECOMMENDED_SIM_EEROS_VERSION} ${REQUIRED_SIM_EEROS_VERSION})
  find_package(sim-eeros ${VERSION_TO_FIND})
  checkIfModuleFound("sim-eeros" "REQUIRED_SIM_EEROS_VERSION")
  checkVersionCompatibility(${sim-eeros_VERSION} ${VERSION_TO_FIND} ${REQUIRED_DEGREE_OF_VERSION_COMPATIBILITY})
endif(USE_SIM)


# Find Flink
if(USE_FLINK)
  find_package(flink)

  setVersionToFind("flink-eeros" "REQUIRED_FLINK_EEROS_VERSION" ${RECOMMENDED_FLINK_EEROS_VERSION} ${REQUIRED_FLINK_EEROS_VERSION})
  find_package(flink-eeros ${VERSION_TO_FIND})
  checkIfModuleFound("flink-eeros" "REQUIRED_FLINK_EEROS_VERSION")
  checkVersionCompatibility(${flink-eeros_VERSION} ${VERSION_TO_FIND} ${REQUIRED_DEGREE_OF_VERSION_COMPATIBILITY})
endif(USE_FLINK)


# Find BBBlueEEROS
if(USE_BBBLUE)
  setVersionToFind("bbblue-eeros" "REQUIRED_BBBLUE_EEROS_VERSION" ${RECOMMENDED_BBBLUE_EEROS_VERSION} ${REQUIRED_BBBLUE_EEROS_VERSION})
  find_package(bbblue-eeros ${VERSION_TO_FIND})
  checkIfModuleFound("bbblue-eeros" "REQUIRED_BBBLUE_EEROS_VERSION")
  checkVersionCompatibility(${bbblue-eeros_VERSION} ${VERSION_TO_FIND} ${REQUIRED_DEGREE_OF_VERSION_COMPATIBILITY})
endif(USE_BBBLUE)


# Find Comedi
if(USE_COMEDI)
  setVersionToFind("comedi-eeros" "REQUIRED_COMEDI_EEROS_VERSION" ${RECOMMENDED_COMEDI_EEROS_VERSION} ${REQUIRED_COMEDI_EEROS_VERSION})
  find_package(comedi-eeros ${VERSION_TO_FIND})
  checkIfModuleFound("comedi-eeros" "REQUIRED_COMEDI_EEROS_VERSION")
  checkVersionCompatibility(${comedi-eeros_VERSION} ${VERSION_TO_FIND} ${REQUIRED_DEGREE_OF_VERSION_COMPATIBILITY})
endif(USE_COMEDI)


# Find ROS
if(USE_ROS)
  setVersionToFind("ros-eeros" "REQUIRED_ROS_EEROS_VERSION" ${RECOMMENDED_ROS_EEROS_VERSION} ${REQUIRED_ROS_EEROS_VERSION})
  find_package(ros-eeros ${VERSION_TO_FIND})
  checkIfModuleFound("ros-eeros" "REQUIRED_ROS_EEROS_VERSION")
  checkVersionCompatibility(${ros-eeros_VERSION} ${VERSION_TO_FIND} ${REQUIRED_DEGREE_OF_VERSION_COMPATIBILITY})
endif(USE_ROS)


add_custom_command(OUTPUT copyHalConfig POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy "${simpleMotorControl_SOURCE_DIR}/*.json" "${simpleMotorControl_BINARY_DIR}")

add_executable(simpleMotorControl SimpleMotorControl.cpp ControlSystem.cpp SMCSafetyProperties.cpp copyHalConfig)
target_link_libraries(simpleMotorControl PRIVATE EEROS::eeros)

install(TARGETS simpleMotorControl DESTINATION bin)

