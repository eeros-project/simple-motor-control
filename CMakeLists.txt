cmake_minimum_required(VERSION 3.5.1)

project(simpleMotorControl)

set(CMAKE_CXX_STANDARD 14)


set(RECOMMENDED_EEROS_VERSION 1.0.0.48)
set(RECOMMENDED_SIM_EEROS_VERSION 1.0.0.0)
set(RECOMMENDED_FLINKLIB_VERSION 1.0.0.0)
set(RECOMMENDED_FLINK_EEROS_VERSION 1.0.0.0)
set(RECOMMENDED_BBBLUE_EEROS_VERSION 1.1.0.0)
set(RECOMMENDED_COMEDI_EEROS_VERSION 1.0.0.0)
set(RECOMMENDED_ROS_EEROS_VERSION 1.0.0.0)


if(CMAKE_BUILD_TYPE MATCHES "Debug|DEBUG")
	set(LOGGER ON)
endif()


include(cmake/versionCheckHelpers.cmake)

# Find EEROS
setVersionToFind("EEROS" "REQUIRED_EEROS_VERSION" ${RECOMMENDED_EEROS_VERSION} ${REQUIRED_EEROS_VERSION})
find_package(EEROS ${VERSION_TO_FIND} EXACT)
checkIfModuleFound("EEROS" "REQUIRED_EEROS_VERSION")


# Find Simulator
if(USE_SIM)
  setVersionToFind("sim-eeros" "REQUIRED_SIM_EEROS_VERSION" ${RECOMMENDED_SIM_EEROS_VERSION} ${REQUIRED_SIM_EEROS_VERSION})
  find_package(sim-eeros ${VERSION_TO_FIND} EXACT)
  checkIfModuleFound("sim-eeros" "REQUIRED_SIM_EEROS_VERSION")
endif(USE_SIM)


# Find Flink
if(USE_FLINK)
#  setVersionToFind("flinklib" "REQUIRED_FLINKLIB_VERSION" ${RECOMMENDED_FLINKLIB_VERSION} ${REQUIRED_FLINKLIB_VERSION})
#  find_package(flinklib ${VERSION_TO_FIND} EXACT)
  find_package(flink)
#  checkIfModuleFound("flinklib" "REQUIRED_FLINKLIB_VERSION")

  setVersionToFind("flink-eeros" "REQUIRED_FLINK_EEROS_VERSION" ${RECOMMENDED_FLINK_EEROS_VERSION} ${REQUIRED_FLINK_EEROS_VERSION})
  find_package(flink-eeros ${VERSION_TO_FIND} EXACT)
  checkIfModuleFound("flink-eeros" "REQUIRED_FLINK_EEROS_VERSION")
endif(USE_FLINK)


# Find BBBlueEEROS
if(USE_BBBLUE)
  setVersionToFind("bbblueeeros" "REQUIRED_BBBLUE_EEROS_VERSION" ${RECOMMENDED_BBBLUE_EEROS_VERSION} ${REQUIRED_BBBLUE_EEROS_VERSION})
  find_package(bbblueeeros ${VERSION_TO_FIND} EXACT)
  checkIfModuleFound("bbblueeeros" "REQUIRED_BBBLUE_EEROS_VERSION")
endif(USE_BBBLUE)


# Find Comedi
if(USE_COMEDI)
  setVersionToFind("comedi-eeros" "REQUIRED_COMEDI_EEROS_VERSION" ${RECOMMENDED_COMEDI_EEROS_VERSION} ${REQUIRED_COMEDI_EEROS_VERSION})
  find_package(comedi-eeros ${VERSION_TO_FIND} EXACT)
  checkIfModuleFound("comedi-eeros" "REQUIRED_COMEDI_EEROS_VERSION")
endif(USE_COMEDI)


# Find ROS
if(USE_ROS)
  setVersionToFind("ros-eeros" "REQUIRED_ROS_EEROS_VERSION" ${RECOMMENDED_ROS_EEROS_VERSION} ${REQUIRED_ROS_EEROS_VERSION})
  find_package(ros-eeros ${VERSION_TO_FIND} EXACT)
  checkIfModuleFound("ros-eeros" "REQUIRED_ROS_EEROS_VERSION")
endif(USE_ROS)


include_directories(${EEROS_INCLUDE_DIR};${EEROS_LIBUCL_INCLUDE_DIR})
link_directories(${EEROS_LIB_DIR};${EEROS_LIBUCL_LINK_DIR})

add_custom_command(OUTPUT copyHalConfig POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy "${simpleMotorControl_SOURCE_DIR}/*.json" "${simpleMotorControl_BINARY_DIR}")

add_executable(simpleMotorControl SimpleMotorControl.cpp ControlSystem.cpp SMCSafetyProperties.cpp copyHalConfig)
target_link_libraries(simpleMotorControl eeros ucl ${CMAKE_DL_LIBS})

