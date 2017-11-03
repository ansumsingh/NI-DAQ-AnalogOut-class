cmake_minimum_required(VERSION 2.7)

project(ContGen-IntClk)

set(NI_folder "your path to/National Instruments/NI-DAQ/DAQmx ANSI C Dev")
set(NI_LIB_DIRS "${NI_folder}/lib/msvc")

include_directories("${NI_folder}/include")
message(STATUS "${NI_LIB_DIRS}/*.lib")
file(GLOB NI_LIBRARIES "${NI_LIB_DIRS}/*.lib")


message(STATUS "NI_LIBRARIES: " ${NI_LIBRARIES})
message(STATUS "NI_FOLDER: " ${NI_folder})


add_executable(signalOutTest signalOutTest.cpp signalOut.h)
 

target_link_libraries(signalOutTest ${NI_LIBRARIES} )
