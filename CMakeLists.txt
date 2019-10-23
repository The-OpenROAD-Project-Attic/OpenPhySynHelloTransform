cmake_minimum_required(VERSION 3.13.4)

project(PhyKnightHelloTransform VERSION 1.0.0 LANGUAGES CXX)

set(TRANSFORM_NAME hello_transform)

find_package(TCL REQUIRED)


if(DEFINED ENV{PHY_HOME_PATH})
set(PHY_HOME_PATH $ENV{PHY_HOME_PATH})
else()
set(PHY_HOME_PATH "/usr/include/phyknight")
endif()

IF(NOT EXISTS ${PHY_HOME_PATH})
message( FATAL_ERROR "Cannot find PhyKnight headers, set PHY_HOME_PATH variable." )
endif()

if(DEFINED ENV{OPENDB_HOME_PATH})
set(OPEDNDB_HOME $ENV{OPENDB_HOME_PATH})
else()
set(OPEDNDB_HOME ${PHY_HOME_PATH}/../external/OpenDB)
endif()

IF(NOT EXISTS ${OPEDNDB_HOME})
message( FATAL_ERROR "Cannot find OpenDB headers, set OPENDB_HOME_PATH variable." )
endif()

if(DEFINED ENV{PHY_LIB_PATH})
set(PHY_LIB_PATH $ENV{PHY_LIB_PATH})
link_directories(${PHY_LIB_PATH})
endif()

if(DEFINED ENV{OPENDB_LIB_PATH})
set(OPENDB_LIB_PATH $ENV{OPENDB_LIB_PATH})
link_directories(${OPENDB_LIB_PATH})
link_directories(${OPENDB_LIB_PATH}/src)
link_directories(${OPENDB_LIB_PATH}/src/db)
link_directories(${OPENDB_LIB_PATH}/src/defin)
link_directories(${OPENDB_LIB_PATH}/src/defout)
link_directories(${OPENDB_LIB_PATH}/src/lefin)
link_directories(${OPENDB_LIB_PATH}/src/lefout)
link_directories(${OPENDB_LIB_PATH}/src/zlib)
link_directories(${OPENDB_LIB_PATH}/src/zutil)
link_directories(${OPENDB_LIB_PATH}/src/tm)
endif()

include_directories(
  src/
  ${PHY_HOME_PATH}/src #To be removed when public headers are cleaned up
  ${PHY_HOME_PATH}/include
  ${PHY_HOME_PATH}/include/PhyKnight
  ${PHY_HOME_PATH}/external/spdlog/include
  ${OPEDNDB_HOME}/include
  ${OPEDNDB_HOME}/src
  ${OPEDNDB_HOME}/src/db
)

add_library(${TRANSFORM_NAME} SHARED src/HelloTransform.cpp)
target_link_libraries(${TRANSFORM_NAME} PhyKnight)
target_link_libraries(${TRANSFORM_NAME}
db
defin
lefin
lefout
lef
defout
zutil
zlib
tm
dl
${TCL_LIBRARY}
)
# set_target_properties( ${TRANSFORM_NAME}
#       PROPERTIES
#       LIBRARY_OUTPUT_DIRECTORY "../bin"
#       )
target_compile_features(${TRANSFORM_NAME} PRIVATE cxx_range_for cxx_auto_type)
target_compile_options(${TRANSFORM_NAME} PRIVATE $<$<CXX_COMPILER_ID:GNU>:-Wall>)
set_property(TARGET ${TRANSFORM_NAME} PROPERTY POSITION_INDEPENDENT_CODE ON)




